#include "VC_Server.h"

AVC_Server::AVC_Server() {
	PrimaryActorTick.bCanEverTick = true;
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
    if (!Receiver->Init(FString("0.0.0.0"), Settings.ServerPort, Settings.BufferSize)) {
        // TODO: Logs
        return false;
    }
    Receiver->UDPReceiver->OnDataReceived().BindUObject(this, &AVC_Server::UDPReceive);
	Receiver->UDPReceiver->Start();

	SendersManager = NewObject<UVC_SendersManager>();
	if (SendersManager == nullptr) {
		// TODO: logs
		return false;
	}

	VoiceBroadcast = NewObject<UVC_VoiceBroadcast>();
	if (VoiceBroadcast == nullptr) {
		// TODO: Logs
		return false;
	}

	InitStatus = true;
	return InitStatus;
};

void AVC_Server::Deinit() {
	InitStatus = false;

	if (Receiver != nullptr) {
        Receiver->Deinit();
    }
	//todo: memory fix
	// if (VoiceBroadcast != nullptr) {
	// 	delete VoiceBroadcast;
	// }
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
			//todo: logs
			return;
		}
		Packet.Channel = NewChannel;
		SendersManager->SendData(Packet, NewChannel);
		return;
	}

	VoiceBroadcast->VoiceBroadcast(Packet, SendersManager);
};

void AVC_Server::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	Deinit();
};

void AVC_Server::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

};
