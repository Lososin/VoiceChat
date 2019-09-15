#include "VC_MicrophoneManager.h"

UVC_MicrophoneManager::UVC_MicrophoneManager() : InitStatus(false) {

};

UVC_MicrophoneManager::~UVC_MicrophoneManager() {

};

bool UVC_MicrophoneManager::Init(int SampleRate) {
	Deinit();

	FVoiceModule& VoiceModule = FVoiceModule::Get();
	if (!VoiceModule.DoesPlatformSupportVoiceCapture()) {
		UE_LOG(VoiceChatLog, Error, TEXT("Microphone Module: Voice Module Doesn't Support"));
		return false;
	}

	if (!VoiceModule.IsAvailable()) {
		UE_LOG(VoiceChatLog, Error, TEXT("Microphone Module: Voice Module Doesn't Available"));
		return false;
	}

	if (!VoiceModule.IsVoiceEnabled()) {
		UE_LOG(VoiceChatLog, Error, TEXT("Microphone Module: Voice Module Doesn't Enabled"));
		return false;
	}

	VoiceCapture = VoiceModule.CreateVoiceCapture(TEXT("Default Device"), SampleRate, 1);
	if (!VoiceCapture.IsValid()) {
		UE_LOG(VoiceChatLog, Error, TEXT("Microphone Module: Voice Capture Componnent is not Valid"));
		return false;
	}

	VoiceCapture->Start();

	UE_LOG(VoiceChatLog, Log, TEXT("Microphone Module: Inited with SampleRate=%d"), SampleRate);
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
		if (AvailableSize > 4410000) {
			isValidBuff = false;
		}
	}

	return TArray<uint8>(buff, AvailableSize);
};

void UVC_MicrophoneManager::SetMicVolume(float Volume) {
    // TODO: SetMicVolume func
	UE_LOG(VoiceChatLog, Error, TEXT("Microphone Module: Not Released (Set Mic Volume)"));
};

void UVC_MicrophoneManager::SetMicThreshold(float Threshold) {
	UVOIPStatics::SetMicThreshold(Threshold);
};

bool UVC_MicrophoneManager::IsInited() const {
	return InitStatus;
};

void UVC_MicrophoneManager::Deinit() {
	if (InitStatus == false) {
		return;
	}

	InitStatus = false;
	if (VoiceCapture.IsValid()) {
		VoiceCapture->Stop();
	}
    VoiceCapture.Reset();

	UE_LOG(VoiceChatLog, Log, TEXT("Microphone Module: Deinited"));
};