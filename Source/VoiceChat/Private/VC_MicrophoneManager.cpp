#include "VC_MicrophoneManager.h"

UVC_MicrophoneManager::UVC_MicrophoneManager() : InitStatus(false) {

};

UVC_MicrophoneManager::~UVC_MicrophoneManager() {
    Deinit();
};

bool UVC_MicrophoneManager::Init(int SampleRate) {
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

TArray<uint8> UVC_MicrophoneManager::GetVoiceBuffer(bool& isValidBuff) {
	isValidBuff = false;
	
	if (InitStatus == false) { 
		return TArray<uint8>();
	}

	uint32 AvailableSize;
	uint8 buff[4410000];
	if (VoiceCapture->GetCaptureState(AvailableSize) == EVoiceCaptureState::Type::Ok) {
		VoiceCapture->GetVoiceData(buff, 4410000, AvailableSize);
		isValidBuff = true;
	}

	return TArray<uint8>(buff, AvailableSize);
};

void UVC_MicrophoneManager::SetMicVolume(float Volume) {
    // TODO: SetMicVolume func
};

void UVC_MicrophoneManager::SetMicThreshold(float Threshold) {
	UVOIPStatics::SetMicThreshold(Threshold);
};

bool UVC_MicrophoneManager::IsInited() const {
	return InitStatus;
};

void UVC_MicrophoneManager::Deinit() {
	InitStatus = false;
	if (VoiceCapture.IsValid()) {
		VoiceCapture->Stop();
	}
    VoiceCapture.Reset();
};