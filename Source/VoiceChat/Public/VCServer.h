#pragma once

#include "CoreMinimal.h"
#include "VCSettings.h"
#include "VCVoicePacket.h"
#include "VCSender.h"
#include "VCSourceInfo.h"
#include "VCServer.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class AVCServer : public AActor {
	GENERATED_BODY()

public:
	AVCServer();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	void SetSettings(FVCSettings _Settings);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	FVCSettings& GetSettings() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	bool Init();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	void Deinit();


	UFUNCTION(BlueprintCallable, Category = "VoiceChat,UDP")
	bool UDPSend(const FVCVoicePacket& Packet, FVCSender Sender);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,UDP")
	void UDPSendBroadcast(const FVCVoicePacket& Packet);

	//UPROPERTY(BlueprintAssignable, Category = "VoiceChat,UDP")
	//FUDPReceiveDelegate UDPReceiveDelegate;

	UFUNCTION(BlueprintImplementableEvent, Category = "VoiceChat,UDP")
	void BPEvent_UDPReceive(const FVCVoicePacket& ReceivedData, const FVCSourceInfo SourceInfo);


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

protected:
	void UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

	bool UDPReceiverInit();

	bool IsAlreadyClient(FVCSourceInfo ClientInfo);

	bool RegClient(FVCSourceInfo ClientInfo);

	bool UnregClient(FVCSender ClientSender);

	int FindClientChannel(FVCSourceInfo ClientInfo);

private:
	TArray<FVCSender> Senders;

	int ChannelsNum = 0;

	FSocket* ListenerSocket;
	FUdpSocketReceiver* UDPReceiver;

	FVCSettings Settings;
	bool IsInitialized = false;
};
