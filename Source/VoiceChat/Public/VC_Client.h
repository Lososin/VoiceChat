#pragma once

#include "CoreMinimal.h"
#include "VoiceChat.h"

#include "VC_Settings.h"
#include "VC_Packet.h"
#include "VC_Sender.h"
#include "VC_Receiver.h"
#include "VC_MicrophoneManager.h"
#include "VC_AudioManager.h"
#include "VC_ChannelAssigner.h"
#include "VC_Client.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class AVC_Client : public AActor {
	GENERATED_BODY()

public:
	AVC_Client();
	~AVC_Client();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Client")
	void SetSettings(FVC_Settings _Settings);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Client")
	FVC_Settings GetSettings() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Client")
	bool Init();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Client")
	void Deinit();

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Client")
	bool UDPSend(FVC_Packet Packet);

	UFUNCTION(BlueprintCallable, Category = "VoiceChatPlugin|Client")
	void SetMicrophoneThreshold(float NewThreshold);

	UFUNCTION(BlueprintImplementableEvent)
	void BPEvent_UDPReceive(const FVC_Packet Packet);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VoiceChatPlugin|Client")
	bool CanPacketBeSet(FVC_Packet Packet);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VoiceChatPlugin|Client")
	FString GetVoiceMeta() const;

	UPROPERTY(BlueprintReadWrite)
	bool bAllowPlayBackLocal = false;

	UPROPERTY(BlueprintReadWrite)
	bool bAllowPlayBackServer = false;

protected:
	virtual void BeginPlay() override;

	void UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

private:

	UVC_MicrophoneManager* MicrophoneManager;
	UVC_AudioManager* AudioManager;
	UVC_Sender* Sender;
	UVC_Receiver* Receiver;

	UVC_ChannelAssigner* ChannelAssigner;
	
	FVC_Settings Settings;
	bool InitStatus = false;
};
