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

	int GetChannelNum(FVC_Address SourceInfo);

	bool CreateNewSender(FString IpSrc, int PortSrc, FString IpDst, int PortDst, int BufferSize);

	bool SendData(FVC_Packet Packet, int Channel);

	TArray<int> GetChannelsArray() const;

	// TODO: delete channel feature
private:
	TArray<FVCSender> Senders;
	int SendersNum = 0;
};

// Wrong Name - this is not "Channels Manager", it's something like SendersManager or otherwise like this
