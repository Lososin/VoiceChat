#include "VC_ChannelsManager.h"

UVC_ChannelsManager::UVC_ChannelsManager() {

};

bool UVC_ChannelsManager::CreateNewChannel(FVCSourceInfo DstAddress, FVCSettings Settings) {
    auto NewSender = NewObject<FVCSender>();

	NewSender->RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool isVal;
	NewSender->RemoteAddress->SetIp(*SourceInfo.IP, isVal);
	if (!isVal) {
		UE_LOG(VoiceChatLog, Error, TEXT("IP is not valid (CreateNewSender)"));
		return false;
	}

	NewSender->RemoteAddress->SetPort(Settings.Port);

	FString SocketName = FString::Printf(TEXT("SNDR_SRV_SOCK_IP_%s_PORT_%d"), *SourceInfo.IP, SourceInfo.Port);	
	NewSender->SenderSocket = FUdpSocketBuilder(SocketName).AsReusable().WithBroadcast();
	if (NewSender->SenderSocket == nullptr) {
		UE_LOG(VoiceChatLog, Error, TEXT("Sender Socket doesn't created (CreateNewSender)"));
		return false;
	}

	int32 BufferSize = Settings.BufferSize;
	NewSender->SenderSocket->SetReceiveBufferSize(BufferSize, BufferSize);
	NewSender->SenderSocket->SetSendBufferSize(BufferSize, BufferSize);

	NewSender->SourceInfo = SourceInfo;

    Channels.Add(NewSender);

	return true;
};

void UVC_ChannelsManager::AssignNewChannelNumber(FVCSender* Sender) {
    Sender->Channel = AllChannels;
    Channels++;
};

int UVC_ChannelsManager::GetChannelNum(FVCSourceInfo SourceInfo) {
    // Find channel
    for (auto& a : Channels) {
        if (a.SourceInfo.IP == SourceInfo.IP &&
                a.SourceInfo.Port == SourceInfo.Port) {
            return a.Channel;
        }
    }

    return -1;
};

TArray<FVCSender>& UVC_ChannelsManager::GetAllSenders() {
	return Channels;
};