#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundWaveProcedural.h"
#include "VC_AudioTrack.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_AudioTrack : public UObject {
	GENERATED_BODY()

public:
	UVC_AudioTrack();
	~UVC_AudioTrack();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,VoiceTrack")
	void Init(int Channel, int SampleRate);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,VoiceTrack")
	void AddWaveData(TArray<uint8> Data);

private:
	TUniquePtr<USoundWaveProcedural> AudioStream;
	int AudioChannel = -1;
};
