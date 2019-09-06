#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "VoiceModule.h"
#include "VC_MicrophonManager.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_MicrophonManager : public UObject {
	GENERATED_BODY()

public:
	UVC_MicrophonManager() {

	};

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,MicrophoneManager")
	bool Init(int SampleRate = 44100) {
		Deinit();

		FVoiceModule& VoiceModule = FVoiceModule::Get();

		if (!VoiceModule.DoesPlatformSupportVoiceCapture()) {
			UE_LOG(VoiceChatLog, Error, TEXT("Voice Module Does not Support"));
			return false;
		}

		if (!VoiceModule.IsAvailable()) {
			UE_LOG(VoiceChatLog, Error, TEXT("Voice Module Does not Available"));
			return false;
		}

		if (!VoiceModule.IsVoiceEnabled()) {
			UE_LOG(VoiceChatLog, Error, TEXT("Voice Module Does not Enable"));
			return false;
		}

		VoiceCapture = VoiceModule.CreateVoiceCapture(TEXT("Default Device"), SampleRate, 1);

		if (!VoiceCapture.IsValid()) {
			return false;
		}

		VoiceCapture->Start();

		InitStatus = true;
		return InitStatus;
	};

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,MicrophoneManager")
    TArray<uint8> GetVoiceBuffer(bool& isValidBuff) {
		isValidBuff = false;
		
		if (!InitStatus || !VoiceCapture->IsCapturing()) {
			return TArray<uint8>();
		}

		uint32 AvailableSize;
		uint8 buff[44100];

		if (VoiceCapture->GetCaptureState(AvailableSize) == EVoiceCaptureState::Type::Ok) {
			VoiceCapture->GetVoiceData(buff, 44100, AvailableSize);
			isValidBuff = true;
		}

		return TArray<uint8>(buff, AvailableSize);
	};

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,MicrophoneManager")
	void SetMicVolume(float Volume = 1.f) {
		TSharedPtr<UAudioComponent> AudioComponent(CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceCaptureComponent")));
		AudioComponent->PitchMultiplier = 0.85f;
		AudioComponent->VolumeMultiplier = Volume;
	};

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,MicrophoneManager")
	void SetMicThreshold(float Threshold = 0.01f) {
		UVOIPStatics::SetMicThreshold(Threshold);
	};

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,MicrophoneManager")
	bool IsInited() {
		return InitStatus;
	};

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,MicrophoneManager")
	void Deinit() {
		InitStatus = false;
		if (VoiceCapture.IsValid()) {
			VoiceCapture->Stop();
		}
	};

private:
	TSharedPtr<IVoiceCapture> VoiceCapture;
	bool InitStatus = false;
};