#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPv4Address.h"
#include "VC_Address.h"
#include "VC_Packet.h"
#include "VC_ReceiverCallback.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UReceiver_Callback : public UObject {
	GENERATED_BODY()

public:
	UReceiver_Callback() {};
	~UReceiver_Callback() {};

    virtual void UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt) {

    };
};