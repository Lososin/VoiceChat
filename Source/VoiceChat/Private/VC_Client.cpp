#include "VC_Client.h"

AVC_Client::AVC_Client() {
	PrimaryActorTick.bCanEverTick = true;

};

AVC_Client::~AVC_Client() {

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
    MicrophoneManager->AddToRoot();
    if (!MicrophoneManager->Init(Settings.SampleRate)) {
        UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Client: Microphone Manager not Inited"));
        return false;
    }

    AudioManager = NewObject<UVC_AudioManager>();
    AudioManager->AddToRoot();
    if (!AudioManager->CreateNewAudio(GetWorld(), Settings.SampleRate, 0)) {     // For Playback
        UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Client: Audio Manager not Inited"));
        return false;
    }
    if (!AudioManager->CreateNewAudio(GetWorld(), Settings.SampleRate, 1)) {     // For Playback
        return false;
    }
    if (!AudioManager->CreateNewAudio(GetWorld(), Settings.SampleRate, 2)) {     // For Playback
        return false;
    }
    if (!AudioManager->CreateNewAudio(GetWorld(), Settings.SampleRate, 3)) {     // For Playback
        return false;
    }

    Sender = NewObject<UVC_Sender>();
    Sender->AddToRoot();
    if (!Sender->Init("0.0.0.0", 0, Settings.ServerIP, Settings.ServerPort, Settings.BufferSize)) {
        UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Client: Sender not Inited"));
        return false;
    }

    Receiver = NewObject<UVC_Receiver>();
    Receiver->AddToRoot();
    if (!Receiver->Init(FString("0.0.0.0"), Settings.ClientPort, Settings.BufferSize)) {
        UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Client: Receiver not Inited"));
        return false;
    }
    Receiver->UDPReceiver->OnDataReceived().BindUObject(this, &AVC_Client::UDPReceive);
    Receiver->UDPReceiver->Start();

    ChannelAssigner = NewObject<UVC_ChannelAssigner>();
    ChannelAssigner->AddToRoot();
    if (ChannelAssigner == nullptr) {
        UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Client: Channel Assigner not Inited"));
        return false;
    }

    UE_LOG(VoiceChatLog, Log, TEXT("VoiceChat Client: Inited"));
    InitStatus = true;
	return InitStatus;
};

void AVC_Client::Deinit() {
    if (InitStatus == false) {
		return;
	}

    InitStatus = false;

    if (MicrophoneManager != nullptr) {
        MicrophoneManager->Deinit();
        delete MicrophoneManager;
    }

    if (AudioManager != nullptr) {
        AudioManager->DeleteAllChannels();
        delete AudioManager;
    }

    if (Receiver != nullptr) {
        Receiver->Deinit();
        delete Receiver;
    }

    if (Sender != nullptr) {
        Sender->Deinit();
        delete Sender;
    }

    if (ChannelAssigner != nullptr) {
        delete ChannelAssigner;
    }

    UE_LOG(VoiceChatLog, Log, TEXT("VoiceChat Client: Deinited"));
};

bool AVC_Client::UDPSend(FVC_Packet Packet) {
	if (InitStatus == false) {
		UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Client: Not Inited (UDPSend)"));
		return false;
	}
		
	FArrayWriter Writer;
	Writer << Packet;

	if (!Sender->SendPacket(Packet)) {
        UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Client: Send Packet Error"));
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
        GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, FString::Printf(TEXT("CHANNEL ASSIGN RECEIVED")));
    }

    if (Packet.Meta == "CHANNELASSIGN" && ChannelAssigner->GetStatus() == VC_ConnectionProtocolStagesClient::AWAIT_CHANNEL) {
        ChannelAssigner->SetChannel(Packet);
        return;
    }

    if (CanPacketBeSet(Packet)) {
        if (ChannelAssigner->GetChannel() == Packet.Channel) {
            if (bAllowPlayBackLocal == false && bAllowPlayBackServer == true) {
                AudioManager->SetData(Packet.VoiceData, 0);
            }
            return;
        }

        AudioManager->SetData(Packet.VoiceData, Packet.Channel);
        return;
    }
};

void AVC_Client::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
};

void AVC_Client::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (InitStatus == false) {
		return;
	}

    if (bAllowPlayBackLocal == true) {
        bool isValid = false;
        auto Data = MicrophoneManager->GetVoiceBuffer(isValid);
        if (isValid == false) {
            return;
        }
        AudioManager->SetData(Data, 0);
        return;
    }

    if (ChannelAssigner->GetStatus() != VC_ConnectionProtocolStagesClient::CHANNEL_ASSIGN) {
        if (ChannelAssigner->GetStatus() == VC_ConnectionProtocolStagesClient::SEND_REQUEST) {
            FVC_Packet NewPacket = ChannelAssigner->GetRequestPacket();
            Sender->SendPacket(NewPacket);
            UE_LOG(VoiceChatLog, Log, TEXT("VoiceChat Client: Channel Request Sended"));
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
        UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Client: Not Inited (SetMicrophoneThreshold)"));
        return;
    }

    MicrophoneManager->SetMicThreshold(NewThreshold);
    //UE_LOG(VoiceChatLog, Error, TEXT("VoiceChat Client: Not Inited (SetMicrophoneThreshold)"));
};