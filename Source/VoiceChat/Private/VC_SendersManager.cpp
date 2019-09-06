#include "VC_SendersManager.h"

UVC_SendersManager::UVC_SendersManager() {

};

bool UVC_SendersManager::CreateNewSender(FVC_Address ClientSrcAddress, FVC_Settings Settings) {
    TUniquePtr<UVC_Sender> NewSender(NewObject<UVC_Sender>());

	NewSender->Init(ClientSrcAddress.IP, ClientSrcAddress.Port, ClientSrcAddress.IP, Settings.ClientPort, Settings.BufferSize);

    Senders.Add(std::move(NewSender));

	return true;
};

int UVC_SendersManager::GetChannelNum(FVC_Address SourceInfo) const {
    // Find channel
    for (auto& a : Senders) {
        if (a->SourceInfo.IP == SourceInfo.IP &&
                a->SourceInfo.Port == SourceInfo.Port) {
            return a->Channel;
        }
    }

    return -1;
};

bool UVC_SendersManager::SendData(FVC_Packet Packet, int Channel) const {
    for (auto& a : Senders) {
        if (a->Channel == Channel) {
            bool Result = a->SendPacket(Packet);
            return Result;
        }
    }

	return false;

};

TArray<int> UVC_SendersManager::GetChannelsArray() const {
    TArray<int> ChArray;
    for (auto& a : Senders) {
        ChArray.Add(a->Channel);
    }
	return ChArray;
};