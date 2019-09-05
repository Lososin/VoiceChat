#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundWaveProcedural.h"
#include "VoiceChat.h"
#include "VC_AudioTrack.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_AudioTrack : public UObject {
	GENERATED_BODY()

public:
	UVC_AudioTrack();
	~UVC_AudioTrack();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,AudioTrack")
	bool Init(int Channel, int SampleRate);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,AudioTrack")
	void AddWaveData(TArray<uint8> AudioData);

private:
	TUniquePtr<USoundWaveProcedural> AudioStream;
	int AudioChannel = -1;
};
