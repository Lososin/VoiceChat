#pragma once

#include "CoreMinimal.h"
#include "VoiceOver.h"
#include "VoiceChatVoiceInfo.generated.h"

USTRUCT(BlueprintType)
struct FVoiceChatVoiceInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	UVoiceOver* VoiceOver = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
    UAudioComponent* AudioComponent = nullptr;

	FVoiceChatVoiceInfo()
	{}

    FVoiceChatVoiceInfo(UVoiceOver* NewVoiceOver, UAudioComponent* NewAudioComponent) : VoiceOver(NewVoiceOver), AudioComponent(NewAudioComponent)
	{}
};