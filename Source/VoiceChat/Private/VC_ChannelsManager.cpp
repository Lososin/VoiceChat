#include "VC_ChannelsManager.h"

UVC_ChannelsManager::UVC_ChannelsManager() {

};

bool UVC_ChannelsManager::CreateNewSender(FString IpSrc, int PortSrc, FString IpDst, int PortDst, int BufferSize) {
    auto NewSender = NewObject<FVCSender>();

	NewSender->RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool isVal;
	NewSender->RemoteAddress->SetIp(*IpDst, isVal);
	if (!isVal) {
		UE_LOG(VoiceChatLog, Error, TEXT("IP is not valid (CreateNewSender)"));
		return false;
	}

	NewSender->RemoteAddress->SetPort(PortDst);

	FString SocketName = FString::Printf(TEXT("SNDR_SRV_SOCK_IP_%s_PORT_%d"), *IpSrc, PortSrc);	
	NewSender->SenderSocket = FUdpSocketBuilder(SocketName).AsReusable().WithBroadcast();
	if (NewSender->SenderSocket == nullptr) {
		UE_LOG(VoiceChatLog, Error, TEXT("Sender Socket doesn't created (CreateNewSender)"));
		return false;
	}

	NewSender->SenderSocket->SetReceiveBufferSize(BufferSize, BufferSize);
	NewSender->SenderSocket->SetSendBufferSize(BufferSize, BufferSize);

	NewSender->SourceInfo = SourceInfo(IpSrc, PortSrc);

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

TArray<FVCSender>& UVC_ChannelsManager::GetAllSenders() const {
	return Channels;
};

FVCSender& UVC_ChannelsManager::GetSender(int Channel) const {
	return Channels.FindByKey(Channel);
};