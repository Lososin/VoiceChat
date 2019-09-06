#pragma once

#include "CoreMinimal.h"
#include "VC_Address.h"
#include "VC_Packet.h"
#include "VC_Settings.h"
#include "VC_Sender.h"
#include "VC_SendersManager.generated.h"


UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_SendersManager : public UObject {
	GENERATED_BODY()

public:
	UVC_SendersManager();

	int GetChannelNum(FVC_Address SourceInfo) const;

	bool CreateNewSender(FVC_Address ClientSrcAddress, FVC_Settings Settings);

	bool SendData(FVC_Packet Packet, int Channel) const;

	TArray<int> GetChannelsArray() const;

	// TODO: delete channel feature
private: 
	TArray<TUniquePtr<UVC_Sender>> Senders;
	int SendersNum = 0;
};
