#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPv4Address.h"
#include "VC_Address.h"
#include "VC_Packet.h"
#include "VC_ReceiverCallback.h"
#include "VC_Receiver.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_Receiver : public UObject {
	GENERATED_BODY()

public:
	UVC_Receiver();
	~UVC_Receiver();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|Sender")
	bool Init(FString Ip, int Port, int BufferSize);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|Sender")
	void Deinit();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|Sender")
	bool IsInited() const;

    // TODO: Think about callback (mb queu)
    FUdpSocketReceiver* UDPReceiver;
private:
    UReceiver_Callback* Callback;

    FSocket* ListenerSocket;
	bool InitStatus;
};