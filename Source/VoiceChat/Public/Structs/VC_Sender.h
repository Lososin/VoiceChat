#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPv4Address.h"
#include "VCSourceInfo.h"
#include "VC_Sender.generated.h"

USTRUCT(BlueprintType)
struct FVC_Sender {
	GENERATED_USTRUCT_BODY()
	
    TSharedPtr<FInternetAddr> RemoteAddress;
	TUniquePtr<TFSocket> SenderSocket;

	FVC_EndpointInfo SourceInfo;

	int Channel = 0;

	bool Init(FString IpSrc, int PortSrc, FString IpDst, int PortDst, int BufferSize) {
		RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

		bool isVal;
		RemoteAddress->SetIp(*IpDst, isVal);
		if (!isVal) {
			UE_LOG(VoiceChatLog, Error, TEXT("IP is not valid (Sender Init)"));
			return false;
		}

		RemoteAddress->SetPort(PortDst);

		FString SocketName = FString::Printf(TEXT("SNDR_SRV_SOCK_IP_%s_PORT_%d"), *IpSrc, PortSrc);	
		SenderSocket = FUdpSocketBuilder(SocketName).AsReusable().WithBroadcast();
		if (SenderSocket == nullptr) {
			UE_LOG(VoiceChatLog, Error, TEXT("Sender Socket doesn't created (Sender Init)"));
			return false;
		}

		SenderSocket->SetReceiveBufferSize(BufferSize, BufferSize);
		SenderSocket->SetSendBufferSize(BufferSize, BufferSize);

		SourceInfo = SourceInfo(IpSrc, PortSrc);

		return true;
	};

	void SetChannel(int _Channel) {
		Channel = _Channel;
	};

	bool SendPacket(FVC_Packet& Packet) {
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

	~FVC_Sender() {
		SenderSocket->Close();
	};
};