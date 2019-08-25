#pragma once

#include "CoreMinimal.h"
#include "VCVoicePacket.h"
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

	ProtocolName GetProtocolName(FVCVoicePacket Packet);
};
