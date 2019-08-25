#include "VC_ProtocolSwaper.h"

UVC_ProtocolSwapper::UVC_ProtocolSwapper() {

};

ProtocolName UVC_ProtocolSwapper::GetProtocol(FVCVoicePacket Packet) {
    if (Packet.CurrentChannel == -1) {
        return ProtocolName::CHANNEL_ASSIGNER;
    } else {
        return ProtocolName::VOICE_BROADCAST;
    }

    return ProtocolName::NONE;
}