#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPv4Address.h"
#include "VC_Address.h"
#include "VC_Packet.h"
#include "VC_Sender.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_Sender : public UObject {
	GENERATED_BODY()

public:
	UVC_Sender();
	~UVC_Sender();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|Sender")
	bool Init(FString IpSrc, int PortSrc, FString IpDst, int PortDst, int BufferSize, int NewChannel = -1);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|Sender")
	void Deinit();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|Sender")
	bool IsInited() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|Sender")
	int GetChannelNumber() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|Sender")
	FVC_Address GetSourceInfo() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|BaseClasses|Sender")
	bool SendPacket(FVC_Packet Packet) const;

private:
    TSharedPtr<FInternetAddr> RemoteAddress;
	TUniquePtr<FSocket> SenderSocket;

	FVC_Address SourceInfo;
	int Channel;

	bool InitStatus;
};