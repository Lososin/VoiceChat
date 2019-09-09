#include "VC_Sender.h"

UVC_Sender::UVC_Sender() : Channel(-1), InitStatus(false) {

};
	
UVC_Sender::~UVC_Sender() {
	Deinit();
};


bool UVC_Sender::Init(FString IpSrc, int PortSrc, FString IpDst, int PortDst, int BufferSize, int NewChannel) {
	Deinit();
	
    RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool isVal;
	RemoteAddress->SetIp(*IpDst, isVal);
	if (!isVal) {
		UE_LOG(VoiceChatLog, Error, TEXT("IP is not valid (Sender Init)"));
		return false;
	}

	RemoteAddress->SetPort(PortDst);

	FString SocketName = FString::Printf(TEXT("SNDR_SRV_SOCK_IP_%s_PORT_%d"), *IpSrc, PortSrc);	
	SenderSocket.Reset(FUdpSocketBuilder(SocketName).AsReusable().WithBroadcast());
	if (!SenderSocket.IsValid()) {
		UE_LOG(VoiceChatLog, Error, TEXT("Sender Socket doesn't created (Sender Init)"));
		return false;
	}

    // TODO : will see on &NewSize
	SenderSocket->SetReceiveBufferSize(BufferSize, BufferSize);
	SenderSocket->SetSendBufferSize(BufferSize, BufferSize);

	SourceInfo = FVC_Address(IpSrc, PortSrc);

	Channel = NewChannel;

	InitStatus = true;
	return InitStatus;
};

void UVC_Sender::Deinit() {
	InitStatus = false;
	Channel = -1;
	if (SenderSocket.IsValid()) {
		SenderSocket->Close();
	}
	RemoteAddress.Reset();
};

bool UVC_Sender::IsInited() const {
	return InitStatus;
};

int UVC_Sender::GetChannelNumber() const {
    return Channel;
};

FVC_Address UVC_Sender::GetSourceInfo() const {
	return SourceInfo;
};

bool UVC_Sender::SendPacket(FVC_Packet Packet) const {
	if (InitStatus == false) {
		return false;
	}

	FArrayWriter Writer;
	Writer << Packet;

	int32 BytesSent = 0;
	SenderSocket->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *RemoteAddress);
	if (BytesSent <= 0) {
		UE_LOG(VoiceChatLog, Error, TEXT("Socket is valid but the receiver received 0 bytes, make sure it is listening properly!"));
		return false;
	}

	return true;
};
