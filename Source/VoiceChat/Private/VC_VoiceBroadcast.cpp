#include "VC_VoiceBroadcast.h"

UVC_VoiceBroadcast::UVC_VoiceBroadcast() {

};

void UVC_VoiceBroadcast::VoiceBroadcast(FVC_Packet Packet, UVC_SendersManager* Manager) {
    auto ChannelsArray = Manager->GetChannelsArray();

    for (auto& a : ChannelsArray) {
        Manager->SendData(Packet, a);
    }
};