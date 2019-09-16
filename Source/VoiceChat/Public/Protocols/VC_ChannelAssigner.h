#pragma once

#include "CoreMinimal.h"
#include "VC_Address.h"
#include "VC_Sender.h"
#include "VC_Packet.h"
#include "Math/UnrealMathUtility.h"
#include "VoiceChat.h"
#include "VC_ChannelAssigner.generated.h"

UENUM(Blueprintable)
enum class VC_ConnectionProtocolStagesClient : uint8 {
	IDLE = 0,
	SEND_REQUEST,
	AWAIT_CHANNEL,
	CHANNEL_ASSIGN,
	BREAK_CONNECTION,
	ERROR,
};

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_ChannelAssigner : public UObject {
	GENERATED_BODY()

public:
	UVC_ChannelAssigner();
	~UVC_ChannelAssigner();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Protocols|ChannelAssigner")
	int GetChannel() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Protocols|ChannelAssigner")
	FVC_Packet GetRequestPacket();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Protocols|ChannelAssigner")
	VC_ConnectionProtocolStagesClient GetStatus() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Protocols|ChannelAssigner")
	void SetChannel(FVC_Packet Packet);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Protocols|ChannelAssigner")
	void UpdateStatus();

private:
	VC_ConnectionProtocolStagesClient CurrentStage = VC_ConnectionProtocolStagesClient::SEND_REQUEST;
	int Channel = -1;
	int UniqueID;
	int FramesToWait;
};
