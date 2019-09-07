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

    UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|AudioManager")
    bool CreateNewAudio(int SampleRate = 44100, int NewChannel = -1, float Volume = 1.f);

    UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|AudioManager")
    void SetData(TArray<uint8> AudioData, int Channel);

    UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|AudioManager")
    void SetVolume(float NewVolume, int Channel);

private:
    TArray<TUniquePtr<UVC_AudioTrack>> AudioTracks;
};