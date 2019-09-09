#include "VC_AudioTrack.h"

UVC_AudioTrack::UVC_AudioTrack() : Channel(-1), InitStatus(false) {

};

UVC_AudioTrack::~UVC_AudioTrack() {
	Deinit();
};

bool UVC_AudioTrack::Init(const UObject* WorldContextObject, int SampleRate, int NewChannel, float Volume) {
	Deinit();
	// TODO: Log wron context
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

	SoundStream = UGameplayStatics::SpawnSound2D(WorldContextObject, AudioStream, Volume);

	if (SoundStream == nullptr) {
		UE_LOG(VoiceChatLog, Error, TEXT("AudioTrack doesn't Initialize"));
		return false;
	}

	InitStatus = true;
	return InitStatus;
};

void UVC_AudioTrack::Deinit() {
	InitStatus = false;

	// TODO: Memory fix
	// if (SoundStream != nullptr) {
	// 	SoundStream->Stop();
	// 	//delete SoundStream;
	// }

	// if (AudioStream != nullptr) {
	// 	delete AudioStream;
	// }
};

bool UVC_AudioTrack::IsInit() const {
	return InitStatus;
};

int UVC_AudioTrack::GetChannelNumber() const {
	return Channel;
};

bool UVC_AudioTrack::AddWaveData(TArray<uint8> AudioData) {
	if (InitStatus == false) {
		UE_LOG(VoiceChatLog, Error, TEXT("Can't set Wave Data (AudioTrack wasn't initialize)"));
		return false;
	}

	AudioStream->QueueAudio(AudioData.GetData(), AudioData.Num());

	return true;
};

void UVC_AudioTrack::SetVolume(float Volume = 1.f) {
	SoundStream->AdjustVolume(1.f, Volume);
};
