#pragma once

#include "CoreMinimal.h"
#include "VoiceChatSettings.h"
#include "VoiceChatData.h"
#include "VoiceOver.h"
#include "VoiceChatClientInfo.h"
#include "VoiceChatServerInfo.h"

#include "VoiceModule.h"
#include "Kismet/GameplayStatics.h"
#include "VoiceChatServer.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class AVoiceChatServer : public AActor
{
	GENERATED_BODY()

public:
	AVoiceChatServer();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void ConfigureVoiceChat(FVoiceChatSettings OtherSettings);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	bool InitVoiceChat();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void DeinitVoiceChat();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	TArray<uint8> GetVoiceBufferVoiceChat(bool& isValidBuff);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	bool UDPSendVoiceChat(FVoiceChatData DataToSend, FVoiceChatServerInfo Sender);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	bool RegisterNewChannel(int NewChannel);

	UPROPERTY(BlueprintAssignable, Category = "VoiceChat")
	FUDPReceivedVoiceChat UDPReceivedVoiceChat_Client;

	UFUNCTION(BlueprintImplementableEvent)
	void BPEvent_UDPDataReceivedVoiceChat(const FVoiceChatData& ReceivedData, const FString& IP, const int& port);

	UPROPERTY(BlueprintReadOnly)
	FVoiceChatSettings Settings;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	void UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

	bool UDPListenerInit();

	bool RegistrationNewClient(FVoiceChatClientInfo NewClientInfo);

private:
	TArray<FVoiceChatClientInfo> ClientsInfo;

	using FChannelNumber = int;
	TMap<FChannelNumber, FVoiceChatVoiceInfo> VoiceChannels;

	FSocket* ListenerSocket;
	FUdpSocketReceiver* UDPReceiver;
	TArray<FVoiceChatServerInfo> Senders;

	bool IsInitialized = false;
};
