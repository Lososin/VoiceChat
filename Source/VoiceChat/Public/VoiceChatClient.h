#pragma once

#include "CoreMinimal.h"
#include "VoiceChatSettings.h"
#include "VoiceChatData.h"
#include "VoiceModule.h"
#include "Networking/Public/Networking.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPv4Address.h"
#include "Kismet/GameplayStatics.h"
#include "VoiceOver.h"
#include "VoiceChatClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FUDPReceivedVoiceChat,
	const FVoiceChatData&, ReceivedData,
	const FString&, IP,
	const int&, Port
);

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class AVoiceChatClient : public AActor
{
	GENERATED_BODY()

public:
	AVoiceChatClient();

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
	bool UDPSendVoiceChat(FVoiceChatData ToSend);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void RegisterNewVoiceOver();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void SetMicThresholdVoiceChat(const float& Threshold = 0.f);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void SetChannelVolumeVoiceChat(const int& Channel, const float& Volume = 0.f);

	UPROPERTY(BlueprintAssignable, Category = "VoiceChat")
	FUDPReceivedVoiceChat UDPReceivedVoiceChat_Client;

	UFUNCTION(BlueprintImplementableEvent)
	void BPEvent_UDPDataReceivedVoiceChat(const FVoiceChatData& ReceivedData, const FString& IP, const int& port);

	UPROPERTY(BlueprintReadOnly)
	FVoiceChatSettings Settings;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	void UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

	bool VoiceModuleInit();

	bool UDPListenerInit();

	bool UDPSenderInit();

private:

	TSharedPtr<IVoiceCapture> VoiceCapture;

	FSocket* ListenerSocket;
	FSocket* SenderSocket;
	TArray<UVoiceOver*> VoiceOvers;
	TArray<UAudioComponent*> PlayingChannels;

	FUdpSocketReceiver* UDPReceiver;
	TSharedPtr<FInternetAddr> RemoteAddress;

	bool IsInitialized = false;
};
