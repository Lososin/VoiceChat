#include "VC_Receiver.h"

UVC_Receiver::UVC_Receiver() : InitStatus(false) {

};

UVC_Receiver::~UVC_Receiver() {
    Deinit();
};

bool UVC_Receiver::Init(FString Ip, int Port, int BufferSize) {
    FIPv4Address Addr;

	if (!FIPv4Address::Parse(Ip, Addr)) {
        // TODO: Log
        return false;
    }

	FIPv4Endpoint Endpoint(Addr, Port);
	
    // TODO: Buffer size issue
	ListenerSocket = FUdpSocketBuilder("LSTN_CL_SOCK").AsNonBlocking().AsReusable().BoundToEndpoint(Endpoint).WithReceiveBufferSize(BufferSize);
    if (ListenerSocket == nullptr) {
        // TODO: Log
        return false;
    }

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(50);
	UDPReceiver = new FUdpSocketReceiver(ListenerSocket, ThreadWaitTime, TEXT("UDP CLIENT RECEIVER"));
    if (UDPReceiver == nullptr) {
        // TODO: Log
        return false;
    }

	//UDPReceiver->OnDataReceived().BindUObject(this, &UVC_Receiver::UDPReceive);
    //UDPReceiver->Start();

    InitStatus = true;
	return InitStatus;
};

void UVC_Receiver::Deinit() {
    if (InitStatus == true) {
        UDPReceiver->Stop();
        ListenerSocket->Close();
        delete UDPReceiver;
        delete ListenerSocket;
        InitStatus = false;
    }
};

bool UVC_Receiver::IsInited() const {
    return InitStatus;
};

// void UVC_Receiver::UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt) {
//     if (Callback == nullptr) {
//         // TODO: logs
//         return;
//     }

//     Callback->UDPReceive(ArrayReaderPtr, EndPt);
// };

