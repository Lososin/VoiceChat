#include "VC_AudioTrack.h"

UVC_AudioTrack::UVC_AudioTrack() : Channel(-1), InitStatus(false) {

};

UVC_AudioTrack::~UVC_AudioTrack() {
	Deinit();
};

bool UVC_AudioTrack::Init(const UObject* WorldContextObject, int SampleRate, int NewChannel, float Volume) {
	Deinit();

	AudioStream = NewObject<USoundWaveProcedural>();
	if (AudioStream == nullptr) {
		UE_LOG(VoiceChatLog, Error, TEXT("AudioTrack: Audio Stream not Created"));
		return false;
	}

	AudioStream->SetSampleRate(SampleRate);
	AudioStream->SoundGroup = SOUNDGROUP_Voice;
	AudioStream->bLooping = false;
	AudioStream->NumChannels = 1;
	Channel = NewChannel;

	SoundStream = UGameplayStatics::SpawnSound2D(WorldContextObject, AudioStream, Volume);
	if (SoundStream == nullptr) {
		UE_LOG(VoiceChatLog, Error, TEXT("AudioTrack: Sound Stream 2D not Created"));
		return false;
	}

	UE_LOG(VoiceChatLog, Log, TEXT("AudioTrack: Inited"));
	InitStatus = true;
	return InitStatus;
};

void UVC_AudioTrack::Deinit() {
	InitStatus = false;

	if (SoundStream != nullptr) {
		SoundStream->Stop();
		delete SoundStream;
	}

	if (AudioStream != nullptr) {
		delete AudioStream;
	}

	UE_LOG(VoiceChatLog, Log, TEXT("AudioTrack: Deinited"));
};

bool UVC_AudioTrack::IsInit() const {
	return InitStatus;
};

int UVC_AudioTrack::GetChannelNumber() const {
	return Channel;
};

bool UVC_AudioTrack::AddWaveData(TArray<uint8> AudioData) {
	if (InitStatus == false) {
		UE_LOG(VoiceChatLog, Error, TEXT("AudioTrack: Not Inited (AddWaveData)"));
		return false;
	}

	AudioStream->QueueAudio(AudioData.GetData(), AudioData.Num());

	return true;
};

void UVC_AudioTrack::SetVolume(float Volume = 1.f) {
	if (InitStatus == false) {
		UE_LOG(VoiceChatLog, Error, TEXT("AudioTrack: Not Inited (SetVolume)"));
		return;
	}

	SoundStream->AdjustVolume(1.f, Volume);
};
