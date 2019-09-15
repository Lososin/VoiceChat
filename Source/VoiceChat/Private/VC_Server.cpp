#include "VC_Server.h"

AVC_Server::AVC_Server() {
	PrimaryActorTick.bCanEverTick = true;
};

AVC_Server::~AVC_Server() {

};

void AVC_Server::SetSettings(FVC_Settings _Settings) {
	Settings = _Settings;
};

FVC_Settings AVC_Server::GetSettings() const {
	return Settings;
};

bool AVC_Server::Init() {
	Deinit();

	Receiver = NewObject<UVC_Receiver>();
	Receiver->AddToRoot();
    if (!Receiver->Init(FString("0.0.0.0"), Settings.ServerPort, Settings.BufferSize)) {
        UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Server: Receiver not Created"));
        return false;
    }
    Receiver->UDPReceiver->OnDataReceived().BindUObject(this, &AVC_Server::UDPReceive);
	Receiver->UDPReceiver->Start();

	SendersManager = NewObject<UVC_SendersManager>();
	SendersManager->AddToRoot();
	if (SendersManager == nullptr) {
		UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Server: Senders Mannager not Created"));
		return false;
	}

	VoiceBroadcast = NewObject<UVC_VoiceBroadcast>();
	VoiceBroadcast->AddToRoot();
	if (VoiceBroadcast == nullptr) {
		UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Server: Voice Broadcast not Created"));
		return false;
	}

	UE_LOG(VoiceChatLog, Log, TEXT("VoiceChat Server: Inited"));
	InitStatus = true;
	return InitStatus;
};

void AVC_Server::Deinit() {
	if (InitStatus == false) {
		return;
	}

	InitStatus = false;

	if (Receiver != nullptr) {
        Receiver->Deinit();
		delete Receiver;
    }

	if (VoiceBroadcast != nullptr) {
		delete VoiceBroadcast;
	}

	UE_LOG(VoiceChatLog, Log, TEXT("VoiceChat Server: Deinited"));
};

void AVC_Server::UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt) {
	if (InitStatus == false) {
		return;
	}

	FVC_Address ClientInfo(FVC_Address(EndPt.Address.ToString(), EndPt.Port));

	FVC_Packet Packet;
	*ArrayReaderPtr << Packet;

	BPEvent_UDPReceive(Packet, ClientInfo);

	if (Packet.Meta == FString("CHANNELASSIGN")) {
		int NewChannel = AllChannels;
		AllChannels++;
		if (!SendersManager->CreateNewSender(ClientInfo, Settings, NewChannel)) {
			return;
		}
		UE_LOG(VoiceChatLog, Log, TEXT("VoiceChat Server: Assigned New Channel=%d, with UniqueID=%d (Address=%s:%d)"), NewChannel, Packet.UniqueID, *EndPt.Address.ToString(), EndPt.Port);
		Packet.Channel = NewChannel;
		SendersManager->SendData(Packet, NewChannel);
		return;
	}

	VoiceBroadcast->VoiceBroadcast(Packet, SendersManager);
};

void AVC_Server::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
};

void AVC_Server::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

};
