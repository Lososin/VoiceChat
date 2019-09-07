#pragma once

#include "CoreMinimal.h"
#include "VC_Address.h"
#include "VC_Packet.h"
#include "VC_Settings.h"
#include "VC_Sender.h"
#include "VC_SendersManager.generated.h"

//TODO : It can't dependend on the Channel

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_SendersManager : public UObject {
	GENERATED_BODY()

public:
	UVC_SendersManager();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|SendersManager")
	bool CreateNewSender(FVC_Address ClientSrcAddress, FVC_Settings Settings, int ChannelNumber);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|SendersManager")
	int GetChannelNumber(FVC_Address SourceInfo) const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|SendersManager")
	bool SendData(FVC_Packet Packet, int Channel) const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|SendersManager")
	TArray<int> GetChannelsArray() const;

	// TODO: delete channel feature
private: 
	TArray<TUniquePtr<UVC_Sender>> Senders;
};
