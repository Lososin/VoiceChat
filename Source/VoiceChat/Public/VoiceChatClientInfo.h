#pragma once

#include "CoreMinimal.h"
#include "VoiceChatClientInfo.generated.h"

USTRUCT(BlueprintType)
struct FVoiceChatClientInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	FString ClientIP = "127.0.0.1";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int ClientPort = 9999;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int Channel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	FString Meta = "Meta2";

	FVoiceChatClientInfo()
	{}
};