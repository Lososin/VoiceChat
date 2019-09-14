#include "VC_ChannelAssigner.h"

UVC_ChannelAssigner::UVC_ChannelAssigner() {
    UE_LOG(VoiceChatLog, Log, TEXT("ChannelAssigner: Current Status is SEND_REQUEST"));
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
    return FVC_Packet(FString("CHANNELASSIGN"), TArray<uint8>());
};

VC_ConnectionProtocolStagesClient UVC_ChannelAssigner::GetStatus() const {
    return CurrentStage;
};

void UVC_ChannelAssigner::SetChannel(int NewChannel) {
    Channel = NewChannel;
    CurrentStage = VC_ConnectionProtocolStagesClient::CHANNEL_ASSIGN;
    UE_LOG(VoiceChatLog, Log, TEXT("ChannelAssigner: Current Status is CHANNEL_ASSIGNED"));
    UE_LOG(VoiceChatLog, Log, TEXT("ChannelAssigner: Assigned channel %d"), Channel);
};

void UVC_ChannelAssigner::UpdateStatus() {
    if (CurrentStage == VC_ConnectionProtocolStagesClient::AWAIT_CHANNEL) {
        if (FramesToWait <= 0) {
            CurrentStage = VC_ConnectionProtocolStagesClient::SEND_REQUEST;
        }
        FramesToWait--;
    }
};