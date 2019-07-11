#pragma once

#include "CoreMinimal.h"
#include "VoiceChat.h"
#include "Sound/SoundWaveProcedural.h"
#include "VoiceOver.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVoiceOver : public UObject
{
	GENERATED_BODY()

public:
	UVoiceOver();
	~UVoiceOver();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void VoiceOverInit(int SampleRate);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void AddWaveData(TArray<uint8> data);

	UPROPERTY(BlueprintReadOnly)
	USoundWaveProcedural* AudioStream;
};
