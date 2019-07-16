#pragma once

#include "CoreMinimal.h"
#include "VCSettings.h"
#include "VCVoicePacket.h"
#include "VCSender.h"
#include "VCSourceInfo.h"

#include "VoiceModule.h"
#include "Kismet/GameplayStatics.h"
#include "VCServer.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class AVCServer : public AActor {
	GENERATED_BODY()

public:
	AVoiceChatServer();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	void SetSettings(FVCSettings _Settings);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	FVCSettings& GetSettings() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	bool Init();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	void Deinit();


	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Audio")
	bool RegisterNewClient(FVCSourceInfo NewClientInfo);


	UFUNCTION(BlueprintCallable, Category = "VoiceChat,UDP")
	bool UDPSend(FVoiceChatData DataToSend, FVoiceChatServerInfo Sender);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,UDP")
	void UDPSendBroadcast(FVoiceChatData DataToSend);

	//UPROPERTY(BlueprintAssignable, Category = "VoiceChat,UDP")
	//FUDPReceiveDelegate UDPReceiveDelegate;

	UFUNCTION(BlueprintImplementableEvent, Category = "VoiceChat,UDP")
	void BPEvent_UDPReceive(const FVoiceChatData& ReceivedData, const FVCSourceInfo Info);

	UPROPERTY(BlueprintReadOnly)
	FVCSettings Settings;


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

protected:
	void UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

	bool UDPReceiverInit();

	bool RegClient(FVoiceChatClientInfo NewClientInfo);

	bool UnregClient(FVoiceChatClientInfo NewClientInfo);

private:
	TArray<FVCSender> Senders;

	int ChannelsNum = 0;

	TArray<

	FSocket* ListenerSocket;
	FUdpSocketReceiver* UDPReceiver;

	bool IsInitialized = false;
};
