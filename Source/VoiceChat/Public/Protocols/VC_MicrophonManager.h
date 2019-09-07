#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "VoiceModule.h"
#include "VC_MicrophonManager.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_MicrophonManager : public UObject {
	GENERATED_BODY()

public:
	UVC_MicrophonManager();
	~UVC_MicrophonManager();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|MicrophoneManager")
	bool Init(int SampleRate = 44100);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|MicrophoneManager")
	void Deinit();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|MicrophoneManager")
	bool IsInited() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|MicrophoneManager")
    TArray<uint8> GetVoiceBuffer(bool& isValidBuff) const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|MicrophoneManager")
	void SetMicVolume(float Volume = 1.f);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Managers|MicrophoneManager")
	void SetMicThreshold(float Threshold = 0.01f);

private:
	TSharedPtr<IVoiceCapture> VoiceCapture;
	bool InitStatus;
};