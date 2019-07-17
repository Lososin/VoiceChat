#pragma once

#include "CoreMinimal.h"
#include "VCVoiceTrack.h"
#include "VCVoiceChannel.generated.h"

USTRUCT(BlueprintType)
struct FVCVoiceChannel {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	UVCVoiceTrack* VoiceTrack = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
    UAudioComponent* AudioComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int Channel = 0;

	int LifeTime = 100000;

	FVCVoiceChannel() {}

	FVCVoiceChannel(UVCVoiceTrack* NewVoiceTrack, UAudioComponent* NewAudioComponent, int NewChannel) :
			VoiceTrack(NewVoiceTrack), AudioComponent(NewAudioComponent), Channel(NewChannel) {
	}
};