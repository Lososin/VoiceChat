#include "UVC_SendersManager.h"

UVC_SendersManager::UVC_SendersManager() {

};

bool UVC_SendersManager::CreateNewSender(FVC_Address ClientSrcAddress, FVC_Settings Settings) {
    auto NewSender = NewObject<FVC_Sender>();

	NewSender->Init(Settings)

    Channels.Add(NewSender);

	return true;
};

void UVC_SendersManager::AssignNewChannelNumber(FVCSender* Sender) {
    Sender->Channel = AllChannels;
    Channels++;
};

int UVC_SendersManager::GetChannelNum(FVCSourceInfo SourceInfo) {
    // Find channel
    for (auto& a : Channels) {
        if (a.SourceInfo.IP == SourceInfo.IP &&
                a.SourceInfo.Port == SourceInfo.Port) {
            return a.Channel;
        }
    }

    return -1;
};

TArray<FVCSender>& UVC_SendersManager::GetAllSenders() const {
	return Channels;
};

FVC_Sender& UVC_SendersManager::GetSender(int Channel) const {
	return Channels.FindByKey(Channel);
};