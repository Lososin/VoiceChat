#include "VCClient.h"

AVCClient::AVCClient() {
	PrimaryActorTick.bCanEverTick = true;
}

void AVCClient::SetSettings(FVCSettings _Settings) {
	Settings = _Settings;
}

FVCSettings AVCClient::GetSettings() const {
	return Settings;
}

bool AVCClient::Init() {
	Deinit();
	
	if (VoiceModuleInit() && UDPListenerInit() && UDPSenderInit()) {
		UDPReceiver->Start();
		VoiceCapture->Start();
		IsInitialized = true;
		return true;
	}

	return false;
}

bool AVCClient::VoiceModuleInit() {
	FVoiceModule& VoiceModule = FVoiceModule::Get();

	if (!VoiceModule.DoesPlatformSupportVoiceCapture()) {
		UE_LOG(VoiceChatLog, Error, TEXT("Voice Module Does not Support"));
		return false;
	}

	if (!VoiceModule.IsAvailable()) {
		UE_LOG(VoiceChatLog, Error, TEXT("Voice Module Does not Available"));
		return false;
	}

	if (!VoiceModule.IsVoiceEnabled()) {
		UE_LOG(VoiceChatLog, Error, TEXT("Voice Module Does not Enable"));
		return false;
	}

	VoiceCapture = VoiceModule.CreateVoiceCapture(TEXT("Default Device"), Settings.SampleRate, 1);
	UVOIPStatics::SetMicThreshold(0);

	return true;
}

bool AVCClient::UDPListenerInit() {
	FIPv4Address Addr;
	FIPv4Address::Parse(Settings.ServerIP, Addr);
	FIPv4Endpoint Endpoint(Addr, Settings.ClientPort);
	int32 BufferSize = Settings.BufferSize;
	ListenerSocket = FUdpSocketBuilder("LSTN_CL_SOCK").AsNonBlocking().AsReusable().BoundToEndpoint(Endpoint).WithReceiveBufferSize(BufferSize);
	if (ListenerSocket == nullptr) {
		return false;
	}

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	UDPReceiver = new FUdpSocketReceiver(ListenerSocket, ThreadWaitTime, TEXT("UDP CLIENT RECEIVER"));
	if (UDPReceiver == nullptr) {
		return false;
	}

	UDPReceiver->OnDataReceived().BindUObject(this, &AVCClient::UDPReceive);

	return true;
}

bool AVCClient::UDPSenderInit() {
	Sender.RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool isVal;
	Sender.RemoteAddress->SetIp(*Settings.ServerIP, isVal);
	if (!isVal) {
		return false;
	}

	Sender.RemoteAddress->SetPort(Settings.ServerPort);

	Sender.SenderSocket = FUdpSocketBuilder("SNDR_CL_SOCK").AsReusable().WithBroadcast();
	if (Sender.SenderSocket == nullptr) {
		return false;
	}

	int32 BufferSize = Settings.BufferSize;
	Sender.SenderSocket->SetReceiveBufferSize(BufferSize, BufferSize);
	Sender.SenderSocket->SetSendBufferSize(BufferSize, BufferSize);

	return true;
}

void AVCClient::Deinit() {
	if (Sender.SenderSocket) {
		Sender.SenderSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Sender.SenderSocket);
	}
	
	delete UDPReceiver;

	if (ListenerSocket != nullptr) {
		ListenerSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerSocket);
	}

	if (VoiceCapture) {
		VoiceCapture->Stop();
	}

	IsInitialized = false;
}


TArray<uint8> AVCClient::GetVoiceBuffer(bool& isValidBuff) {
	uint32 AvailableSize;
	isValidBuff = false;
	uint8 buff[44100];
	if (VoiceCapture->GetCaptureState(AvailableSize) == EVoiceCaptureState::Type::Ok) {
		VoiceCapture->GetVoiceData(buff, 44100, AvailableSize);
		isValidBuff = true;
	}

	return TArray<uint8>(buff, AvailableSize);
}

void AVCClient::SetVoiceBuffer(FVCVoicePacket Packet) {
	if (IsNewChannel(Packet.CurrentChannel)) {
		RegNewChannel(Packet.CurrentChannel);
	}

	for (auto& ch : VoiceChannels) {
		if (ch.Channel = Packet.CurrentChannel) {
			ch.VoiceTrack->AddWaveData(Packet.VoiceData);
		}
	}
}

bool AVCClient::UDPSend(FVCVoicePacket Packet) {
	if (!IsInitialized) {
		UE_LOG(VoiceChatLog, Error, TEXT("Voice Client is not Initialized"));
		return false;
	}
		
	FArrayWriter Writer;
	Writer << Packet;

	int32 BytesSent = 0;
	SenderSocket->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *Sender.RemoteAddress);

	if (BytesSent <= 0) {
		UE_LOG(VoiceChatLog, Error, TEXT("Socket is valid but the receiver received 0 bytes, make sure it is listening properly!"));
		return false;
	}

	return true;
}

void AVCClient::UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt) {
	FVCVoicePacket Packet;
	*ArrayReaderPtr << Packet;

	BPEvent_UDPDataReceivedVoiceChat(Packet);
}

void AVCClient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DeinitVoiceChat();
}

bool AVCClient::RegNewChannel(int NewChannel) {
	for (auto& channel : VoiceChannels) {
		if (channel.Channel == NewChannel) {
			UE_LOG(VoiceChatLog, Error, TEXT("Channel %d already exist"), &NewChannel);
			return false;
		}
	}

	NewVoiceTrack = NewObject<UVCVoiceTrack>();
	if (NewVoiceTrack == nullptr) {
		//TODO : LOG
		return false;
	}
	NewVoiceTrack->VoiceOverInit(Settings.SampleRate);

	UAudioComponent* NewAudio = UGameplayStatics::SpawnSound2D(GetWorld(), NewVoiceOver->AudioStream, 10.0);
	if (NewAudio == nullptr) {
		//TODO : LOG
		return false;
	}

	VoiceChannels.Add(FVoiceChatVoiceInfo(NewVoiceOver, NewAudio, NewChannel));
	return true;
}

void AVCClient::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!IsInitialized) 
		return;
}

void AVCClient::SetMicThreshold(const float& Threshold) {
	UVOIPStatics::SetMicThreshold(Threshold);
}

void AVCClient::SetVolumeLevel(const int& Channel, const float& Volume) {
	for (auto& channel : VoiceChannels) {
		if (channel.Channel == Channel) {
			channel.AudioComponent->SetVolumeMultiplier(Volume);
			return;
		}
	}

	UE_LOG(VoiceChatLog, Error, TEXT("Channel %d is not exist (Volume Set)"), &Channel);
}