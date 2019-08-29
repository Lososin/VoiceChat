#include "VC_VoiceBroadcast.h"

UVC_VoiceBroadcast::UVC_VoiceBroadcast() {

};

void UVC_VoiceBroadcast::VoiceBroadcast(FVCVoicePacket Packet, const UVC_ChannelsManager& Manager) {
    auto Senders = Manager.GetAllSenders();

    for (auto& a : Senders) {
        if (a.Channel != Packet.CurrentChannel) {
            a.SendPacket(Packet);
        }
    }
};