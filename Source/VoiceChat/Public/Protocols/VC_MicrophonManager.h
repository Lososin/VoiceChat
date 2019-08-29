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

		if (!VoiceCapture) {
			return false;
		}

		VoiceCapture->Start();

		IsInited = false;
		return true;
	};

    TArray<uint8> GetVoiceBuffer(bool& isValidBuff) {
		isValidBuff = false;
		
		if (!IsInited) {
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

	void SetMicVolume(float Volume = 1.f) {
		
	};

	void SetMicThreshold(float Threshold = 0.01f) {
		UVOIPStatics::SetMicThreshold(Threshold);
	};

	bool IsInited() {
		return InitStatus;
	};

	void Deinit() {
		InitStatus = false;
		if (VoiceCapture) {
			VoiceCapture->Stop();
		}
	};

private:
	TSharedPtr<IVoiceCapture> VoiceCapture;
	bool InitStatus = false;
};