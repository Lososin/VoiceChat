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

	void SetSendersManager(TSharedPtr<UVC_SendersManager>& Manager);

	void VoiceBroadcast(FVC_Packet Packet, const UVC_SendersManager& Manager);

private:
	TSharedPtr<UVC_SendersManager> SendersManager;
};
