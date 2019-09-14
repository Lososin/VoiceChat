#include "VC_SendersManager.h"

UVC_SendersManager::UVC_SendersManager() {

};

UVC_SendersManager::~UVC_SendersManager() {
    DeleteAllChannels();
};

bool UVC_SendersManager::CreateNewSender(FVC_Address ClientSrcAddress, FVC_Settings Settings, int ChannelNumber) {
    TUniquePtr<UVC_Sender> NewSender(NewObject<UVC_Sender>());
    NewSender->AddToRoot();
    //TODO: Check if already exist
	if (!NewSender->Init(ClientSrcAddress.IP, ClientSrcAddress.Port, ClientSrcAddress.IP, Settings.ClientPort, Settings.BufferSize, ChannelNumber)) {
        UE_LOG(VoiceChatLog, Error, TEXT("Senders Manager: NewSender not Created"));
        return false;
    }

    Senders.Add(std::move(NewSender));

    UE_LOG(VoiceChatLog, Log, TEXT("Senders Manager: NewSender Created"));
    UE_LOG(VoiceChatLog, Log, TEXT("Senders Manager: All Senders is %d"), Senders.Num());
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

    UE_LOG(VoiceChatLog, Warning, TEXT("Senders Manager: Senders with IPsrc=%s and PORTsrc=%d doesn't exist (GetChannelsNumber)"), *SourceInfo.IP, SourceInfo.Port);
    return -1;
};

bool UVC_SendersManager::SendData(FVC_Packet Packet, int Channel) const {
    for (auto& a : Senders) {      
        if (a->GetChannelNumber() == Channel) {
            bool Result = a->SendPacket(Packet);
            return Result;
        }
    }

    UE_LOG(VoiceChatLog, Warning, TEXT("Senders Manager: Channel %d doesn't exist"), Channel);
	return false;
};

TArray<int> UVC_SendersManager::GetChannelsArray() const {
    TArray<int> ChArray;
    for (auto& a : Senders) {
        ChArray.Add(a->GetChannelNumber());
    }

	return ChArray;
};

void UVC_SendersManager::DeleteAllChannels() {
    Senders.Empty();
    UE_LOG(VoiceChatLog, Log, TEXT("Senders Manager: All Senders Deleted"));
};