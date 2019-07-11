#pragma once

#include "CoreMinimal.h"
#include "VoiceChatSettings.generated.h"

USTRUCT(BlueprintType)
struct FVoiceChatSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	FString ServerIP = "127.0.0.1";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int ServerPort = 8001;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int ClientPort = 8002;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int BufferSize = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int SampleRate = 44100;

	FVoiceChatSettings()
	{}
};
