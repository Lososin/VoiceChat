#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundWaveProcedural.h"
#include "VoiceOver.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVCVoiceTrack : public UObject {
	GENERATED_BODY()

public:
	UVCVoiceTrack();
	~UVCVoiceTrack();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,VoiceTrack")
	void Init(int SampleRate);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,VoiceTrack")
	void AddWaveData(TArray<uint8> Data);

	UPROPERTY(BlueprintReadOnly)
	USoundWaveProcedural* AudioStream;
};
