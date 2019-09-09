#pragma once

#include "CoreMinimal.h"
#include "VC_Address.h"
#include "VC_Packet.h"
#include "VC_SendersManager.h"
#include "VC_VoiceBroadcast.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_VoiceBroadcast : public UObject {
	GENERATED_BODY()

public:
	UVC_VoiceBroadcast();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Protocols|VoiceBroadcast")
	void VoiceBroadcast(FVC_Packet Packet, UVC_SendersManager* Manager);
};
