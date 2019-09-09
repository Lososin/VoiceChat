#include "VC_SendersManager.h"

UVC_SendersManager::UVC_SendersManager() {

};

bool UVC_SendersManager::CreateNewSender(FVC_Address ClientSrcAddress, FVC_Settings Settings, int ChannelNumber) {
    TUniquePtr<UVC_Sender> NewSender(NewObject<UVC_Sender>());

	if (!NewSender->Init(ClientSrcAddress.IP, ClientSrcAddress.Port, ClientSrcAddress.IP, Settings.ClientPort, Settings.BufferSize, ChannelNumber)) {
        // TODO: Log parametrs
        return false;
    }

    //Senders.Add(NewSender);

	return true;
};

int UVC_SendersManager::GetChannelNumber(FVC_Address SourceInfo) const {
    // Find channel
    for (auto& a : Senders) {
        if (a->GetSourceInfo().IP == SourceInfo.IP &&
                a->GetSourceInfo().Port == SourceInfo.Port) {
            return a->GetChannelNumber();
        }
    }

    // TODO: Log parametrs
    return -1;
};

bool UVC_SendersManager::SendData(FVC_Packet Packet, int Channel) const {
    for (auto& a : Senders) {
        if (a->GetChannelNumber() == Channel) {
            bool Result = a->SendPacket(Packet);
            return Result;
        }
    }

    // TODO: Log parametrs
	return false;
};

TArray<int> UVC_SendersManager::GetChannelsArray() const {
    TArray<int> ChArray;
    for (auto& a : Senders) {
        ChArray.Add(a->GetChannelNumber());
    }

	return ChArray;
};