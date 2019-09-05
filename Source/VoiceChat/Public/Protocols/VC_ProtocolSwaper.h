#pragma once

#include "CoreMinimal.h"
#include "VC_Packet.h"
#include "VC_ProtocolSwaper.generated.h"

enum class ProtocolName : int {
    NONE = 0,
    CHANNEL_ASSIGNER,
    VOICE_BROADCAST,
};

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_ProtocolSwaper : public UObject {
	GENERATED_BODY()

public:
	UVC_ProtocolSwaper();

	ProtocolName GetProtocolName(FVC_Packet Packet);
};
