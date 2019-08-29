#pragma once

#include "CoreMinimal.h"
#include "VCSourceInfo.h"
#include "VCVoicePacket.h"
#include "VC_ChannelsManager.h"
#include "VC_VoiceBroadcast.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_VoiceBroadcast : public UObject {
	GENERATED_BODY()

public:
	UVC_VoiceBroadcast();

	void SetSendersManager(TSharedPtr<UVC_SendersManager> Manager);

	static void VoiceBroadcast(FVCVoicePacket Packet, const UVC_ChannelsManager& Manager);

private:
	TSharedPtr<UVC_SendersManager> SendersManager;
};
