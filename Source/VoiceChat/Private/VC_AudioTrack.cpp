#pragma once

#include "VC_AudioTrack.h"

UVC_AudioTrack::UVC_AudioTrack() {
};

UVC_AudioTrack::~UVC_AudioTrack() {
};

bool UVC_AudioTrack::Init(int Channel, int SampleRate) {
	AudioStream.Reset(NewObject<USoundWaveProcedural>());
	AudioStream->SetSampleRate(SampleRate);
	AudioStream->SoundGroup = SOUNDGROUP_Voice;
	AudioStream->bLooping = false;
	AudioStream->NumChannels = 1;
	AudioChannel = Channel;

	// TODO: Errors Handler
	return true;
}

void UVC_AudioTrack::AddWaveData(TArray<uint8> AudioData) {
	if (!AudioStream.IsValid()) {
		UE_LOG(VoiceChatLog, Error, TEXT("Voice Over is not Initialized"));
		return;
	}

	AudioStream->QueueAudio(AudioData.GetData(), AudioData.Num());
}