#include "VoiceChatServer.h"

AVoiceChatServer::AVoiceChatServer() {
	PrimaryActorTick.bCanEverTick = true;
}

void AVoiceChatServer::ConfigureVoiceChat(FVoiceChatSettings OtherSettings) {
	Settings = OtherSettings;
}

bool AVoiceChatServer::InitVoiceChat() {
	DeinitVoiceChat();

	if (UDPListenerInit()) {
		UDPReceiver->Start();
		IsInitialized = true;
		return true;
	}

	return false;
}

bool AVoiceChatServer::UDPListenerInit() {
return false;
}

bool AVoiceChatServer::RegistrationNewClient(FVoiceChatClientInfo NewClientInfo) {
	FVoiceChatServerInfo NewServerInfo;

	NewServerInfo.ClientInfo = NewClientInfo;

	//TODO::Bring this to Server Info Method
	NewServerInfo.RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool isVal;
	NewServerInfo.RemoteAddress->SetIp(*NewClientInfo.ClientIP, isVal);
	if (!isVal) {
		UE_LOG(VoiceChatLog, Error, TEXT("IP is not valid (RegNewClient)"));
		return false;
	}

	NewServerInfo.RemoteAddress->SetPort(Settings.ClientPort);

	FString SocketName = FString::Printf(TEXT("SNDR_SRV_SOCK_IP_%s_PORT_%d"), *NewClientInfo.ClientIP, NewClientInfo.ClientPort);	
	NewServerInfo.SenderSocket = FUdpSocketBuilder(SocketName).AsReusable().WithBroadcast();

	int32 BufferSize = 2 * 1024 * 1024;
	NewServerInfo.SenderSocket->SetReceiveBufferSize(BufferSize, BufferSize);
	NewServerInfo.SenderSocket->SetSendBufferSize(BufferSize, BufferSize);

	return true;
}

void AVoiceChatServer::DeinitVoiceChat() {
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

bool AVoiceChatServer::UDPSendVoiceChat(FVoiceChatData DataToSend, FVoiceChatServerInfo Sender) {
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

void AVoiceChatServer::UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt) {

}

void AVoiceChatServer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DeinitVoiceChat();
}

bool AVoiceChatServer::RegisterNewChannel(int NewChannel) {
return false;
}

void AVoiceChatServer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!IsInitialized) 
		return;
}
