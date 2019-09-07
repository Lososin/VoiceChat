#include "VC_MicrophonManager.h"

UVC_MicrophonManager::UVC_MicrophonManager() : InitStatus(false) {

};

UVC_MicrophonManager::~UVC_MicrophonManager() {
    Deinit();
};

bool UVC_MicrophonManager::Init(int SampleRate) {
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

TArray<uint8> UVC_MicrophonManager::GetVoiceBuffer(bool& isValidBuff) const {
	isValidBuff = false;
	
	if (InitStatus == false) {
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

void UVC_MicrophonManager::SetMicVolume(float Volume) {
    // TODO: SetMicVolume func
	TSharedPtr<UAudioComponent> AudioComponent(CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceCaptureComponent")));
	AudioComponent->PitchMultiplier = 0.85f;
	AudioComponent->VolumeMultiplier = Volume;
};

void UVC_MicrophonManager::SetMicThreshold(float Threshold) {
	UVOIPStatics::SetMicThreshold(Threshold);
};

bool UVC_MicrophonManager::IsInited() const {
	return InitStatus;
};

void UVC_MicrophonManager::Deinit() {
	InitStatus = false;
	if (VoiceCapture.IsValid()) {
		VoiceCapture->Stop();
	}
    VoiceCapture.Reset();
};