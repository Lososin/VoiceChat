#pragma once

#include "CoreMinimal.h"
#include "VoiceChat.h"

#include "VCSettings.h"
#include "VCVoicePacket.h"
#include "VCSender.h"
#include "VCVoiceChannel.h"

#include "Kismet/GameplayStatics.h"
#include "VoiceModule.h"
#include "VCClient.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class AVCClient : public AActor {
	GENERATED_BODY()

public:
	AVCClient();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	void SetSettings(FVCSettings _Settings);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	FVCSettings GetSettings() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	bool Init();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void Deinit();


	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	TArray<uint8> GetVoiceBuffer(bool& isValidBuff);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void SetVoiceBuffer(FVCVoicePacket Packet);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void SetVolumeLevel(const int& Channel, const float& Volume = 0.f);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void SetMicThreshold(const float& Threshold = 0.f);


	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	bool UDPSend(FVCVoicePacket Packet);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	bool RegNewChannel(int NewChannel);

	//UPROPERTY(BlueprintAssignable, Category = "VoiceChat")
	//FUDPReceivedVoiceChat UDPReceivedVoiceChat_Client;

	UFUNCTION(BlueprintImplementableEvent)
	void BPEvent_UDPReceive(const FVCVoicePacket& Packet);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	USoundWaveProcedural* NewAudioStream2;

protected:
	virtual void BeginPlay() override;

	void UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

	bool VoiceModuleInit();

	bool UDPListenerInit();

	bool UDPSenderInit();

	bool IsNewChannel(int Channel);

private:

	TSharedPtr<IVoiceCapture> VoiceCapture;

	TArray<FVCVoiceChannel> VoiceChannels;

	TArray<USoundWaveProcedural*> Waves;
	TArray<UAudioComponent*> Compon;

	FSocket* ListenerSocket;
	FUdpSocketReceiver* UDPReceiver;
	FVCSender Sender;

	FVCSettings Settings;
	bool IsInitialized = false;
};
