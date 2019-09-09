#include "VC_AudioManager.h"
#pragma optimize("", off)
UVC_AudioManager::UVC_AudioManager() {

};

bool UVC_AudioManager::CreateNewAudio(const UObject* WorldContextObject, int SampleRate, int NewChannel, float Volume) {
    UVC_AudioTrack* NewAudio = NewObject<UVC_AudioTrack>();

    if (NewAudio == nullptr) {
        // TODO: LOG
        return false;
    }

    if (!NewAudio->Init(WorldContextObject, SampleRate, NewChannel, Volume)) {
        // TODO: LOG
        return false;
    }

    AudioTracks.Add(NewAudio);
    return true;
};

void UVC_AudioManager::SetData(TArray<uint8> AudioData, int Channel) {
    for (auto& a : AudioTracks) {
        if (a->GetChannelNumber() == Channel) {
            a->AddWaveData(AudioData);
            break;
        }
    }

    // TODO: log this channel didn't find
};

void UVC_AudioManager::SetVolume(float Volume, int Channel) {
    for (auto& a : AudioTracks) {
        if (a->GetChannelNumber() == Channel) {
            a->SetVolume(Volume);
            break;
        }
    }

    // TODO: log this channel didn't find
};

void UVC_AudioManager::DeleteAllChannels() {
    AudioTracks.Empty();
};

#pragma optimize("", on)