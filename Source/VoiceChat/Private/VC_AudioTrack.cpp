#pragma once

#include "VC_AudioTrack.h"

UVC_AudioTrack::UVC_AudioTrack() {
}

UVC_AudioTrack::~UVC_AudioTrack() {
}

void UVC_AudioTrack::Init(int SampleRate) {
	if (AudioStream != nullptr) {
		delete AudioStream;
	}

	AudioStream = NewObject<USoundWaveProcedural>();

	if (SampleRate <= 0) {
		UE_LOG(VoiceChatLog, Error, TEXT("Wrong Sample Rate"));
		return;
	}

	AudioStream->SetSampleRate(SampleRate);
	AudioStream->SoundGroup = SOUNDGROUP_Voice;
	AudioStream->bLooping = false;
	AudioStream->NumChannels = 1;
}

void UVC_AudioTrack::AddWaveData(TArray<uint8> data) {
	if (AudioStream == nullptr) {
		UE_LOG(VoiceChatLog, Error, TEXT("Voice Over is not Initialized"));
		return;
	}

	AudioStream->QueueAudio(data.GetData(), data.Num());
}