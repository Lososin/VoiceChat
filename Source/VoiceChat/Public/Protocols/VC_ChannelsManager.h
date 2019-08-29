#pragma once

#include "CoreMinimal.h"
#include "VCSourceInfo.h"
#include "VCSettings.h"
#include "VC_Sender.h"
#include "VC_ChannelsManager.generated.h"


UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_ChannelsManager : public UObject {
	GENERATED_BODY()

public:
	UVC_ChannelsManager();

	int GetChannelNum(FVCSourceInfo SourceInfo);

	bool CreateNewSender(FString IpSrc, int PortSrc, FString IpDst, int PortDst, int BufferSize);

	void AssignNewChannelNumber(FVCSender* Sender);

	TArray<FVCSender>& GetAllSenders() const;

	FVCSender& GetSender(int Channel) const;

	// TODO: delete channel feature
private:
	TArray<FVCSender> Senders;
	int SendersNum = 0;
};

// Wrong Name - this is not "Channels Manager", it's something like SendersManager or otherwise like this
