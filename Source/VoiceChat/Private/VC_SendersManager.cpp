#include "VC_SendersManager.h"

UVC_SendersManager::UVC_SendersManager() {

};

bool UVC_SendersManager::CreateNewSender(FVC_Address ClientSrcAddress, FVC_Settings Settings) {
    // FVC_Sender* NewSender = NewObject<FVC_Sender>();

	// NewSender->Init(ClientSrcAddress.IP, ClientSrcAddress.Port, ClientSrcAddress.IP, Settings.ClientPort, Settings.BufferSize);

    // Senders.Add(*NewSender);

	return true;
};

int UVC_SendersManager::GetChannelNum(FVC_Address SourceInfo) {
    // // Find channel
    // for (auto& a : Senders) {
    //     if (a.SourceInfo.IP == SourceInfo.IP &&
    //             a.SourceInfo.Port == SourceInfo.Port) {
    //         return a.Channel;
    //     }
    // }

    return -1;
};