#pragma once

#include "CoreMinimal.h"
#include "VCSourceInfo.h"
#include "VC_ChannelAssigner.generated.h"

// enum class VC_ConnectionProtocolStagesClient : int {
// 	IDLE = 0,
// 	SEND_REQUEST,
// 	AWAIT_CHANNEL,
// 	CHANNEL_ASSIGN,
// 	BREAK_CONNECTION,
// 	ERROR,
// };

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_ChannelAssigner : public UObject {
	GENERATED_BODY()

public:
	UVC_ChannelAssigner();

	int GetChannel(FVCSourceInfo SourceInfo);

private:
	TArray<FVCSourceInfo> Channels;
	int AllChannels = 0;
};