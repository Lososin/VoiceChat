#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "VC_AudioTrack.h"
#include "VC_AudioManager.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_AudioManager : public UObject {
	GENERATED_BODY()

public:
	UVC_AudioManager();

	bool Init();

    bool CreateNewAudio();

    void SetData();

    void Deinit();

private:
    TArray<UVC_AudioTrack> VoiceTracks;
	bool InitStatus = false;
};