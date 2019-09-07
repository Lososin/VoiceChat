#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundWaveProcedural.h"
#include "VoiceChat.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "VC_AT.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class AVC_AT : public AActor {
	GENERATED_BODY()

public:
	AVC_AT() {};

	~AVC_AT() {};

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|AudioTrack")
	bool Init(int SampleRate = 44100, int NewChannel = -1, float Volume = 1.f) {
	    AudioStream = NewObject<USoundWaveProcedural>();
	    if (AudioStream == nullptr) {
	    	UE_LOG(VoiceChatLog, Error, TEXT("AudioTrack doesn't Initialize"));
	    	return false;
	    }

	    AudioStream->SetSampleRate(SampleRate);
	    AudioStream->SoundGroup = SOUNDGROUP_Voice;
	    AudioStream->bLooping = false;
	    AudioStream->NumChannels = 1;

        UGameplayStatics::PlaySound2D(GetWorld(), AudioStream, 10);
	    // Sound = UGameplayStatics::SpawnSound2D(GetWorld(), AudioStream, Volume);

	    // if (Sound == nullptr) {
	    // 	UE_LOG(VoiceChatLog, Error, TEXT("AudioTrack doesn't Initialize"));
	    // 	return false;
	    // }

	    // auto NewSound = UGameplayStatics::CreateSound2D(GetWorld(), AudioStream.Get());

	    return true;
    };

    UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|AudioTrack")
	bool AddWaveData(TArray<uint8> AudioData) {
        if (AudioStream == nullptr) {
            return false;
        }

        AudioStream->QueueAudio(AudioData.GetData(), AudioData.Num());
        return true;
    };

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundWaveProcedural* AudioStream;
private:
	UAudioComponent* Sound;
};
