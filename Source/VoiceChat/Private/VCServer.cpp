#include "VCServer.h"

AVCServer::AVCServer() {
	PrimaryActorTick.bCanEverTick = true;
}

void AVCServer::SetSettings(FVCSettings OtherSettings) {
	Settings = _Settings;
}

FVCSettings& AVCServer::GetSettings() const {
	return Settings;
}

bool AVCServer::Init() {
	Deinit();

	if (UDPReceiverInit()) {
		IsInitialized = true;
		return true;
	}

	return false;
}

void AVCServer::Deinit() {
	for (auto& sender : Senders) {
		if (sender.SenderSocket) {
			sender.SenderSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(sender.SenderSocket);
		}
	}
	
	delete UDPReceiver;

	if (ListenerSocket != nullptr) {
		ListenerSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerSocket);
	}

	IsInitialized = false;
}

bool AVCServer::UDPReceiverInit() {
	FIPv4Address SrvAddress;
	FIPv4Address::Parse("0.0.0.0", SrvAddress);
	FIPv4Endpoint SrvEndpoint(SrvAddress, Settings.ServerPort);
	int32 BufferSize = 2 * 1024 * 1024;

	ListenerSocket = FUdpSocketBuilder("LSTN_SRV_SOCK").AsNonBlocking().AsReusable().BoundToEndpoint(Endpoint).WithReceiveBufferSize(BufferSize);
	if (ListenerSocket == nullptr) {
		//TODO: Log
		return false;
	}

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	UDPReceiver = new FUdpSocketReceiver(ListenerSocket, ThreadWaitTime, TEXT("UDP SRV RECEIVER"));
	if (UDPReceiver == nullptr) {
		//TODO: Log
		return false;
	}

	UDPReceiver->OnDataReceived().BindUObject(this, &AVoiceChatClient::UDPReceive);
	UDPReceiver->Start();

	return true;
}

bool AVCServer::RegisterNewClient(FVCSourceInfo NewClientInfo) {
	FVCSender NewSender;

	//TODO::Bring this to Server Info Method
	NewSender.RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool isVal;
	NewSender.RemoteAddress->SetIp(*NewClientInfo.ClientIP, isVal);
	if (!isVal) {
		UE_LOG(VoiceChatLog, Error, TEXT("IP is not valid (RegNewClient)"));
		return false;
	}

	NewSender.RemoteAddress->SetPort(Settings.ClientPort);

	FString SocketName = FString::Printf(TEXT("SNDR_SRV_SOCK_IP_%s_PORT_%d"), *NewClientInfo.ClientIP, NewClientInfo.ClientPort);	
	NewSender.SenderSocket = FUdpSocketBuilder(SocketName).AsReusable().WithBroadcast();
	if (NewSender.SenderSocket == nullptr) {
		//TODO: Log
		return false;
	}

	int32 BufferSize = Settings.BufferSize;
	NewSender.SenderSocket->SetReceiveBufferSize(BufferSize, BufferSize);
	NewSender.SenderSocket->SetSendBufferSize(BufferSize, BufferSize);

	return true;
}

bool AVCServer::UDPSend(const FVCVoicePacket& Packet, FVCSender Sender) {
	if (!IsInitialized) {
		UE_LOG(VoiceChatLog, Error, TEXT("Voice Server is not Initialized"));
		return false;
	}

	FArrayWriter Writer;
	Writer << DataToSend;

	int32 BytesSent = 0;
	Sender.SenderSocket->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *Sender.RemoteAddress);

	if (BytesSent <= 0) {
		UE_LOG(VoiceChatLog, Error, TEXT("Socket is valid but the receiver received 0 bytes, make sure it is listening properly!"));
		return false;
	}

	return true;
}

void AVCServer::UDPSendBroadcast(const FVCVoicePacket& Packet) {
	for ()
}

void AVCServer::UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt) {

}

void AVCServer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DeinitVoiceChat();
}

void AVCServer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!IsInitialized) 
		return;
}
