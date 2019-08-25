#include "VC_ChannelAssigner.h"

UVC_ChannelAssigner::UVC_ChannelAssigner() {

};

int UVC_ChannelAssigner::GetChannel(FVCSourceInfo SourceInfo) {
    // Find channel
    for (int i = 0; i < Channels.Num(); i++) {
        if (Channels[i].IP == SourceInfo.IP &&
                Channels[i].Port == SourceInfo.Port) {
            return i;
        }
    }

    // Or store, and allocate another one
    Channels.Push(SourceInfo);
    AllChannels++;

    return AllChannels - 1;
}