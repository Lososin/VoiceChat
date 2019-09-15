#include "VC_ChannelAssigner.h"

UVC_ChannelAssigner::UVC_ChannelAssigner() {
    UniqueID = FMath::RandRange(0, 1000000);
    UE_LOG(VoiceChatLog, Log, TEXT("ChannelAssigner: Current Status is SEND_REQUEST, UniqueID=%d"), UniqueID);
};

UVC_ChannelAssigner::~UVC_ChannelAssigner() {
    Channel = -1;
    CurrentStage = VC_ConnectionProtocolStagesClient::SEND_REQUEST;
};

int UVC_ChannelAssigner::GetChannel() const {
    return Channel;
};

FVC_Packet UVC_ChannelAssigner::GetRequestPacket() {
    CurrentStage = VC_ConnectionProtocolStagesClient::AWAIT_CHANNEL;
    FramesToWait = 100;
    UE_LOG(VoiceChatLog, Log, TEXT("ChannelAssigner: Current Status is AWAIT_CHANNEL"));
    return FVC_Packet(FString("CHANNELASSIGN"), TArray<uint8>(), UniqueID);
};

VC_ConnectionProtocolStagesClient UVC_ChannelAssigner::GetStatus() const {
    return CurrentStage;
};

void UVC_ChannelAssigner::SetChannel(FVC_Packet Packet) {
    if (Packet.UniqueID != UniqueID) {
        return;
    }

    Channel = Packet.Channel;
    CurrentStage = VC_ConnectionProtocolStagesClient::CHANNEL_ASSIGN;
    UE_LOG(VoiceChatLog, Log, TEXT("ChannelAssigner: Current Status is CHANNEL_ASSIGNED, Channel is %d"), Channel);
};

void UVC_ChannelAssigner::UpdateStatus() {
    if (CurrentStage == VC_ConnectionProtocolStagesClient::AWAIT_CHANNEL) {
        if (FramesToWait <= 0) {
            CurrentStage = VC_ConnectionProtocolStagesClient::SEND_REQUEST;
        }
        FramesToWait--;
    }
};