#include "VCServer.h"

AVCServer::AVCServer() {
	PrimaryActorTick.bCanEverTick = true;
}

void AVCServer::SetSettings(FVCSettings _Settings) {
	Settings = _Settings;
}

FVCSettings AVCServer::GetSettings() const {
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
	
	ChannelsNum = 0;
	IsInitialized = false;
}

bool AVCServer::UDPReceiverInit() {
	FIPv4Address SrvAddress;
	FIPv4Address::Parse("0.0.0.0", SrvAddress);
	FIPv4Endpoint SrvEndpoint(SrvAddress, Settings.ServerPort);
	int32 BufferSize = Settings.BufferSize;

	ListenerSocket = FUdpSocketBuilder("LSTN_SRV_SOCK").AsNonBlocking().AsReusable().BoundToEndpoint(SrvEndpoint).WithReceiveBufferSize(BufferSize);
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

	UDPReceiver->OnDataReceived().BindUObject(this, &AVCServer::UDPReceive);
	UDPReceiver->Start();

	return true;
}

bool AVCServer::IsAlreadyClient(FVCSourceInfo ClientInfo) {
	for (auto& sndr : Senders) {
		if (//sndr.RemoteAddress->GetIp() == ClientInfo.IP && ================================================================================================
				sndr.SrcPort == ClientInfo.Port) {
			return true;
		}
	}

	return false;
}

bool AVCServer::RegClient(FVCSourceInfo ClientInfo) {
	FVCSender NewSender;

	//TODO::Bring this to Server Info Method
	NewSender.RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool isVal;
	NewSender.RemoteAddress->SetIp(*ClientInfo.IP, isVal);
	if (!isVal) {
		UE_LOG(VoiceChatLog, Error, TEXT("IP is not valid (RegNewClient)"));
		return false;
	}

	NewSender.RemoteAddress->SetPort(Settings.ClientPort);

	FString SocketName = FString::Printf(TEXT("SNDR_SRV_SOCK_IP_%s_PORT_%d"), *ClientInfo.IP, ClientInfo.Port);	
	NewSender.SenderSocket = FUdpSocketBuilder(SocketName).AsReusable().WithBroadcast();
	if (NewSender.SenderSocket == nullptr) {
		//TODO: Log
		return false;
	}

	int32 BufferSize = Settings.BufferSize;
	NewSender.SenderSocket->SetReceiveBufferSize(BufferSize, BufferSize);
	NewSender.SenderSocket->SetSendBufferSize(BufferSize, BufferSize);

	NewSender.Channel = ChannelsNum;
	NewSender.SrcPort = ClientInfo.Port;
	ChannelsNum++;

	Senders.Add(NewSender);

	return true;
}

bool AVCServer::UnregClient(FVCSender ClientSender) {

	ChannelsNum--;
	return false;
}

int AVCServer::FindClientChannel(FVCSourceInfo ClientInfo) {
	for (auto& sndr : Senders) {
		if (//sndr.RemoteAddress.GetIp() == ClientInfo.IP &&
				sndr.SrcPort == ClientInfo.Port) {
			return sndr.Channel;
		}
	}

	return -1;
}

bool AVCServer::UDPSend(FVCVoicePacket Packet, FVCSender Sender) {
	if (!IsInitialized) {
		UE_LOG(VoiceChatLog, Error, TEXT("Voice Server is not Initialized"));
		return false;
	}

	FArrayWriter Writer;
	Writer << Packet;

	int32 BytesSent = 0;
	Sender.SenderSocket->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *Sender.RemoteAddress);

	if (BytesSent <= 0) {
		UE_LOG(VoiceChatLog, Error, TEXT("Socket is valid but the receiver received 0 bytes, make sure it is listening properly!"));
		return false;
	}

	return true;
}

void AVCServer::UDPSendBroadcast(const FVCVoicePacket& Packet, const FVCSourceInfo SourceInfo) {
	for (auto& sndr : Senders) {
		if (sndr.SrcPort != SourceInfo.Port) {
			UDPSend(Packet, sndr);
		}
	}
}

void AVCServer::UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt) {
	FVCSourceInfo ClientInfo(FVCSourceInfo(EndPt.Address.ToString(), EndPt.Port));

	if (!IsAlreadyClient(ClientInfo)) {
		//TODO: Added log
		RegClient(ClientInfo);
	}

	FVCVoicePacket Packet;
	*ArrayReaderPtr << Packet;
	Packet.ChannelsNum = ChannelsNum;
	Packet.CurrentChannel = FindClientChannel(ClientInfo);

	UDPSendBroadcast(Packet, ClientInfo);

	BPEvent_UDPReceive(Packet, ClientInfo);
}

void AVCServer::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	Deinit();
}

void AVCServer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!IsInitialized) {
		return;
	}
}