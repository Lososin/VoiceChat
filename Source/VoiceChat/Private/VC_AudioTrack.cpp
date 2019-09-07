#pragma once

#include "VC_AudioTrack.h"

UVC_AudioTrack::UVC_AudioTrack() : Channel(-1), InitStatus(false) {

};

UVC_AudioTrack::~UVC_AudioTrack() {

};

bool UVC_AudioTrack::Init(int SampleRate, int NewChannel, float Volume) {
	Deinit();

	AudioStream = NewObject<USoundWaveProcedural>();
	if (AudioStream == nullptr) {
		UE_LOG(VoiceChatLog, Error, TEXT("AudioTrack doesn't Initialize"));
		return false;
	}

	AudioStream->SetSampleRate(SampleRate);
	AudioStream->SoundGroup = SOUNDGROUP_Voice;
	AudioStream->bLooping = false;
	AudioStream->NumChannels = 1;
	Channel = NewChannel;

	Sound = UGameplayStatics::SpawnSound2D(GetWorld(), AudioStream, Volume);

	if (Sound == nullptr) {
		UE_LOG(VoiceChatLog, Error, TEXT("AudioTrack doesn't Initialize"));
		return false;
	}

	// auto NewSound = UGameplayStatics::CreateSound2D(GetWorld(), AudioStream.Get());

	InitStatus = true;
	return InitStatus;
};

void UVC_AudioTrack::Deinit() {
	InitStatus = false;

//	Sound.Reset();
//	AudioStream.Reset();
};

bool UVC_AudioTrack::IsInit() const {
	return InitStatus;
};

int UVC_AudioTrack::GetChannelNumber() const {
	return Channel;
};

void UVC_AudioTrack::AddWaveData(TArray<uint8>& AudioData) {
	if (InitStatus == false) {
		UE_LOG(VoiceChatLog, Error, TEXT("Can't set Wave Data (AudioTrack wasn't initialize)"));
		return;
	}

	AudioStream->QueueAudio(AudioData.GetData(), AudioData.Num());
};

void UVC_AudioTrack::SetVolume(float Volume = 1.f) {
	Sound->VolumeMultiplier = Volume;
};