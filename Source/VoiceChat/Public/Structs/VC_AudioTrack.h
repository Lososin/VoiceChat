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

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|AudioTrack")
	bool Init(const UObject* WorldContextObject, int SampleRate = 44100, int NewChannel = -1, float Volume = 1.f);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|AudioTrack")
	void Deinit();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|AudioTrack")
	bool IsInited() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|AudioTrack")
	int GetChannelNumber() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|AudioTrack")
	bool AddWaveData(TArray<uint8> AudioData);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|AudioTrack")
	void SetVolume(float Volume);

private:
	USoundWaveProcedural* AudioStream;
	UAudioComponent* SoundStream;

	int Channel;
	bool InitStatus;
};
