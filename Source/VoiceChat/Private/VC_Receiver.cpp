#include "VC_Receiver.h"

UVC_Receiver::UVC_Receiver() : InitStatus(false) {

};

UVC_Receiver::~UVC_Receiver() {
 
};

bool UVC_Receiver::Init(FString Ip, int Port, int BufferSize) {
    FIPv4Address Addr;

	if (!FIPv4Address::Parse(Ip, Addr)) {
        UE_LOG(VoiceChatLog, Error, TEXT("Receiver: IP Adress is Wrong"));
        return false;
    }

	FIPv4Endpoint Endpoint(Addr, Port);
	
    // TODO: Buffer size issue
	ListenerSocket = FUdpSocketBuilder("LSTN_CL_SOCK").AsNonBlocking().AsReusable().BoundToEndpoint(Endpoint).WithReceiveBufferSize(BufferSize);
    if (ListenerSocket == nullptr) {
        UE_LOG(VoiceChatLog, Error, TEXT("Receiver: Listener Socket not Created"));
        return false;
    }

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(50);
	UDPReceiver = new FUdpSocketReceiver(ListenerSocket, ThreadWaitTime, TEXT("UDP CLIENT RECEIVER"));
    if (UDPReceiver == nullptr) {
        UE_LOG(VoiceChatLog, Error, TEXT("Receiver: UDPReceiver not Created"));
        return false;
    }

    UE_LOG(VoiceChatLog, Log, TEXT("Receiver: Inited with Address=%s:%d, BufferSize=%d"), *Ip, Port, BufferSize);
    InitStatus = true;
	return InitStatus;
};

void UVC_Receiver::Deinit() {
    if (InitStatus == false) {
		return;
	}

    InitStatus = false;
    UDPReceiver->Stop();
    ListenerSocket->Close();
    delete UDPReceiver;
    delete ListenerSocket;
        
    UE_LOG(VoiceChatLog, Log, TEXT("Receiver: Deinited"));
};

bool UVC_Receiver::IsInited() const {
    return InitStatus;
};
