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

	FSocket* SenderSocket;

	FVC_EndpointInfo SourceInfo;

	int Channel = 0;

	bool Init() {

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
};