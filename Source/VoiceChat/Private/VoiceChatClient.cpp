#include "VoiceChatClient.h"

AVoiceChatClient::AVoiceChatClient() {
	PrimaryActorTick.bCanEverTick = true;
}

void AVoiceChatClient::ConfigureVoiceChat(FVoiceChatSettings OtherSettings) {
	Settings = OtherSettings;
}

bool AVoiceChatClient::InitVoiceChat() {
	DeinitVoiceChat();
	
	if (VoiceModuleInit() && UDPListenerInit() && UDPSenderInit()) {
		UDPReceiver->Start();
		VoiceCapture->Start();
		IsInitialized = true;
		return true;
	}

	return false;
}

bool AVoiceChatClient::VoiceModuleInit() {
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

bool AVoiceChatClient::UDPListenerInit() {
	FIPv4Address Addr;
	FIPv4Address::Parse(Settings.ServerIP, Addr);
	FIPv4Endpoint Endpoint(Addr, Settings.ClientPort);
	int32 BufferSize = 2 * 1024 * 1024;
	ListenerSocket = FUdpSocketBuilder("LSTN_CL_SOCK").AsNonBlocking().AsReusable().BoundToEndpoint(Endpoint).WithReceiveBufferSize(BufferSize);
	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	UDPReceiver = new FUdpSocketReceiver(ListenerSocket, ThreadWaitTime, TEXT("UDP CLIENT RECEIVER"));
	UDPReceiver->OnDataReceived().BindUObject(this, &AVoiceChatClient::UDPReceive);

	return true;
}

bool AVoiceChatClient::UDPSenderInit() {
	RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool isVal;
	RemoteAddress->SetIp(*Settings.ServerIP, isVal);
	if (!isVal) {
		return false;
	}

	RemoteAddress->SetPort(Settings.ServerPort);

	SenderSocket = FUdpSocketBuilder("SNDR_CL_SOCK").AsReusable().WithBroadcast();
	int32 BufferSize = 2 * 1024 * 1024;
	SenderSocket->SetReceiveBufferSize(BufferSize, BufferSize);
	SenderSocket->SetSendBufferSize(BufferSize, BufferSize);

	return true;
}

void AVoiceChatClient::DeinitVoiceChat() {
	if (SenderSocket) {
		SenderSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SenderSocket);
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


TArray<uint8> AVoiceChatClient::GetVoiceBufferVoiceChat(bool& isValidBuff) {
	uint32 availableSize;
	isValidBuff = false;
	uint8 buff[44100];
	if (VoiceCapture->GetCaptureState(availableSize) == EVoiceCaptureState::Type::Ok) {
		VoiceCapture->GetVoiceData(buff, 44100, availableSize);
		isValidBuff = true;
	}

	return TArray<uint8>(buff, availableSize);
}

bool AVoiceChatClient::UDPSendVoiceChat(FVoiceChatData ToSend) {
	if (!IsInitialized) {
		UE_LOG(VoiceChatLog, Error, TEXT("Voice Client is not Initialized"));
		return false;
	}
		
	FArrayWriter Writer;
	Writer << ToSend;

	int32 BytesSent = 0;
	SenderSocket->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *RemoteAddress);

	if (BytesSent <= 0) {
		UE_LOG(VoiceChatLog, Error, TEXT("Socket is valid but the receiver received 0 bytes, make sure it is listening properly!"));
		return false;
	}

	return true;
}

void AVoiceChatClient::UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt) {
	FVoiceChatData Data;
	*ArrayReaderPtr << Data;

	BPEvent_UDPDataReceivedVoiceChat(Data, EndPt.Address.ToString(), EndPt.Port);
	UDPReceivedVoiceChat_Client.Broadcast(Data, EndPt.Address.ToString(), EndPt.Port);
}

void AVoiceChatClient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DeinitVoiceChat();
}

bool AVoiceChatClient::RegisterNewChannel(int NewChannel) {
	for (auto& channel : VoiceChannels) {
		if (channel.Key == NewChannel) {
			UE_LOG(VoiceChatLog, Error, TEXT("Channel %d already exist"), &channel);
			return false;
		}
	}

	UVoiceOver* NewVoiceOver = NewObject<UVoiceOver>();
	NewVoiceOver->VoiceOverInit(Settings.SampleRate);
	UAudioComponent* NewAudio = UGameplayStatics::SpawnSound2D(GetWorld(), NewVoiceOver->AudioStream, 10.0);
	
	VoiceChannels.Add(NewChannel, FVoiceChatVoiceInfo(NewVoiceOver, NewAudio));
	return true;
}

void AVoiceChatClient::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!IsInitialized) 
		return;

/*	if (VoiceOvers.Num() > 0) {
		bool isValid = false;
		auto arr = GetVoiceBufferVoiceChat(isValid);

		if (isValid) {
			VoiceOvers[0]->AddWaveData(arr);
		}
	}
	*/
}

void AVoiceChatClient::SetMicThresholdVoiceChat(const float& Threshold) {
	UVOIPStatics::SetMicThreshold(Threshold);
}

void AVoiceChatClient::SetChannelVolumeVoiceChat(const int& Channel, const float& Volume) {
	for (auto& channel : VoiceChannels) {
		if (channel.Key == Channel) {
			channel.Value.AudioComponent->SetVolumeMultiplier(Volume);
			return;
		}
	}

	UE_LOG(VoiceChatLog, Error, TEXT("Channel %d is not exist (Volume Set)"), &Channel);
}