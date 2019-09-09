#include "VC_Client.h"
#pragma optimize("", off)
AVC_Client::AVC_Client() {
	PrimaryActorTick.bCanEverTick = true;

};

void AVC_Client::SetSettings(FVC_Settings _Settings) {
	Settings = _Settings;
};

FVC_Settings AVC_Client::GetSettings() const {
	return Settings;
};

bool AVC_Client::Init() {
	Deinit();

    MicrophoneManager = NewObject<UVC_MicrophoneManager>();
    if (!MicrophoneManager->Init(Settings.SampleRate)) {
        //TODO: Logs
        return false;
    }

    AudioManager = NewObject<UVC_AudioManager>();
    if (!AudioManager->CreateNewAudio(GetWorld(), Settings.SampleRate, 0)) {     // For Playback
        // TODO: Logs
        return false;
    }
    if (!AudioManager->CreateNewAudio(GetWorld(), Settings.SampleRate, 1)) {     // For Playback
        // TODO: Logs
        return false;
    }
    if (!AudioManager->CreateNewAudio(GetWorld(), Settings.SampleRate, 2)) {     // For Playback
        // TODO: Logs
        return false;
    }
    if (!AudioManager->CreateNewAudio(GetWorld(), Settings.SampleRate, 3)) {     // For Playback
        // TODO: Logs
        return false;
    }
    if (!AudioManager->CreateNewAudio(GetWorld(), Settings.SampleRate, 4)) {     // For Playback
        // TODO: Logs
        return false;
    }

    Sender = NewObject<UVC_Sender>();
    if (!Sender->Init("0.0.0.0", 0, Settings.ServerIP, Settings.ServerPort, Settings.BufferSize)) {
        // TODO: Logs
        return false;
    }

    Receiver = NewObject<UVC_Receiver>();
    if (!Receiver->Init(FString("0.0.0.0"), Settings.ClientPort, Settings.BufferSize)) {
        // TODO: Logs
        return false;
    }
    Receiver->UDPReceiver->OnDataReceived().BindUObject(this, &AVC_Client::UDPReceive);
    Receiver->UDPReceiver->Start();

    ChannelAssigner = NewObject<UVC_ChannelAssigner>();
    if (ChannelAssigner == nullptr) {
        //TODO: log
        return false;
    }

    InitStatus = true;
	return InitStatus;
};

void AVC_Client::Deinit() {
    InitStatus = false;

    if (MicrophoneManager != nullptr) {
        MicrophoneManager->Deinit();
    }

    if (AudioManager != nullptr) {
        AudioManager->DeleteAllChannels();
    }

    if (Receiver != nullptr) {
        Receiver->Deinit();
    }

    if (Sender != nullptr) {
        Sender->Deinit();
    }

    if (ChannelAssigner != nullptr) {
        delete ChannelAssigner;
    }
};

bool AVC_Client::UDPSend(FVC_Packet Packet) {
	if (InitStatus == false) {
		UE_LOG(VoiceChatLog, Error, TEXT("Voice Client is not Initialized (UDPSend)"));
		return false;
	}
		
	FArrayWriter Writer;
	Writer << Packet;

	if (!Sender->SendPacket(Packet)) {
        UE_LOG(VoiceChatLog, Error, TEXT("Send Error (Client UDPSend)"));
		return false;
    }

	return true;
};

void AVC_Client::UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt) {
	if (EndPt.Address.ToString() != Settings.ServerIP) {
		return;
	}

	FVC_Packet Packet;
	*ArrayReaderPtr << Packet;

	BPEvent_UDPReceive(Packet);

    if (Packet.Meta == "CHANNELASSIGN") {
        ChannelAssigner->SetChannel(Packet.Channel);
        return;
    }

    if (CanPacketBeSet(Packet)) {
        if (ChannelAssigner->GetChannel() == Packet.Channel) {
            AudioManager->SetData(Packet.VoiceData, 0);
        }

        AudioManager->SetData(Packet.VoiceData, Packet.Channel);
        return;
    }
};

void AVC_Client::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	Deinit();
};

void AVC_Client::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (InitStatus == false) {
		return;
	}

    if (ChannelAssigner->GetStatus() != VC_ConnectionProtocolStagesClient::CHANNEL_ASSIGN) {
        if (ChannelAssigner->GetStatus() == VC_ConnectionProtocolStagesClient::SEND_REQUEST) {
            FVC_Packet NewPacket = ChannelAssigner->GetRequestPacket();
            Sender->SendPacket(NewPacket);
            //todo: logs
        } else if (ChannelAssigner->GetStatus() == VC_ConnectionProtocolStagesClient::AWAIT_CHANNEL) {
            ChannelAssigner->UpdateStatus();
        }

        return;
    } 

    bool isValid = false;
    auto Data = MicrophoneManager->GetVoiceBuffer(isValid);
    if (isValid == false) {
        return;
    }

    if (bAllowPlayBack == true) {
        AudioManager->SetData(Data, 0);
    }

    FVC_Packet NewPacket(GetVoiceMeta(), Data);
    NewPacket.Channel = ChannelAssigner->GetChannel();
    Sender->SendPacket(NewPacket);
};

void AVC_Client::BeginPlay() {
	Super::BeginPlay();
	
};

bool AVC_Client::CanPacketBeSet_Implementation(FVC_Packet Packet) {
    return true;
};

FString AVC_Client::GetVoiceMeta_Implementation() const {
    return FString("VoiceData");
};

void AVC_Client::SetMicrophoneThreshold(float NewThreshold) {
    if (InitStatus == false) {
        //TODO : Log
        return;
    }

    MicrophoneManager->SetMicThreshold(NewThreshold);
};

#pragma optimize("", on)