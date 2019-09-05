#include "VC_ProtocolSwaper.h"

UVC_ProtocolSwaper::UVC_ProtocolSwaper() {

};

ProtocolName UVC_ProtocolSwaper::GetProtocolName(FVC_Packet Packet) {
    if (Packet.Channel == -1) {
        return ProtocolName::CHANNEL_ASSIGNER;
    } else {
        return ProtocolName::VOICE_BROADCAST;
    }

    return ProtocolName::NONE;
}