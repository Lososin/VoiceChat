#include "VC_AudioManager.h"

UVC_AudioManager::UVC_AudioManager() {

};

UVC_AudioManager::~UVC_AudioManager() {
    DeleteAllChannels();
}

bool UVC_AudioManager::CreateNewAudio(const UObject* WorldContextObject, int SampleRate, int NewChannel, float Volume) {
    UVC_AudioTrack* NewAudio = NewObject<UVC_AudioTrack>();
    NewAudio->AddToRoot();
    
    if (NewAudio == nullptr) {
        UE_LOG(VoiceChatLog, Error, TEXT("AudioManager: New Audio Track not Created"));
        delete NewAudio;
        return false;
    }

    if (!NewAudio->Init(WorldContextObject, SampleRate, NewChannel, Volume)) {
        UE_LOG(VoiceChatLog, Error, TEXT("AudioManager: New Audio Track not Inited"));
        return false;
    }

    AudioTracks.Add(NewAudio);
    UE_LOG(VoiceChatLog, Log, TEXT("AudioManager: New Audio Track Created"));
    return true;
};

void UVC_AudioManager::SetData(TArray<uint8> AudioData, int Channel) {
    for (auto& a : AudioTracks) {
        if (a->GetChannelNumber() == Channel) {
            a->AddWaveData(AudioData);
            break;
        }
    }

    UE_LOG(VoiceChatLog, Warning, TEXT("AudioManager: Audio Channel doesn't exist (SetData)"));
};

void UVC_AudioManager::SetVolume(float Volume, int Channel) {
    for (auto& a : AudioTracks) {
        if (a->GetChannelNumber() == Channel) {
            a->SetVolume(Volume);
            break;
        }
    }

    UE_LOG(VoiceChatLog, Warning, TEXT("AudioManager: Audio Channel doesn't exist (SetVolume)"));
};

void UVC_AudioManager::DeleteAllChannels() {
    AudioTracks.Empty();
    UE_LOG(VoiceChatLog, Log, TEXT("AudioManager: All Audio Channels Deleted"));
};
