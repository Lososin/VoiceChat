#pragma once

#include "CoreMinimal.h"
#include "VCSourceInfo.h"
#include "VCSettings.h"
#include "VCSender.h"
#include "VC_ChannelsManager.generated.h"


UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_ChannelsManager : public UObject {
	GENERATED_BODY()

public:
	UVC_ChannelsManager();

	int GetChannelNum(FVCSourceInfo SourceInfo);

	// TODO: to (ipsrc, portsrc, ipdst, portdst, buffsize = default);
	bool CreateNewChannel(FVCSourceInfo DstAddress, FVCSettings Settings);

	void AssignNewChannelNumber(FVCSender* Sender);

	// TODO: to static
	TArray<FVCSender>& GetAllSenders();

	// TODO: delete channel feature
private:
	TArray<FVCSender> Channels;
	int AllChannels = 0;
};
