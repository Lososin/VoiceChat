#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundWaveProcedural.h"
#include "VoiceChat.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "VC_AudioTrack.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_AudioTrack : public UObject {
	GENERATED_BODY()

public:
	UVC_AudioTrack();
	~UVC_AudioTrack();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,AudioTrack")
	bool Init(int Channel, int SampleRate, float Volume);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,AudioTrack")
	void AddWaveData(TArray<uint8> AudioData);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,AudioTrack")
	void SetVolume(float Volume);

private:
	TUniquePtr<USoundWaveProcedural> AudioStream;
	TUniquePtr<UAudioComponent> Sound;
	int AudioChannel = -1;
};
