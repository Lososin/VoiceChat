#include "VC_VoiceBroadcast.h"

UVC_VoiceBroadcast::UVC_VoiceBroadcast() {

};

void UVC_VoiceBroadcast::VoiceBroadcast(FVCVoicePacket Packet, FVCSourceInfo SrcInfo, const UVC_ChannelsManager& Manager) {
    auto Senders = Manager.GetAllSenders();
};