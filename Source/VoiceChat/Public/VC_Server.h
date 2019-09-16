#pragma once

#include "CoreMinimal.h"
#include "VC_Settings.h"
#include "VC_Packet.h"
#include "VC_Receiver.h"
#include "VC_VoiceBroadcast.h"
#include "VC_SendersManager.h"
#include "VoiceChat.h"
#include "VC_Server.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class AVC_Server : public AActor {
	GENERATED_BODY()

public:
	AVC_Server();
	~AVC_Server();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	void SetSettings(FVC_Settings _Settings);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	FVC_Settings GetSettings() const;

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	bool Init();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat,Initialization")
	void Deinit();

	UFUNCTION(BlueprintImplementableEvent, Category = "VoiceChat,UDP")
	void BPEvent_UDPReceive(const FVC_Packet& ReceivedData, const FVC_Address SourceInfo);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

protected:
	void UDPReceive(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

private:
	UVC_SendersManager* SendersManager;
    UVC_Receiver* Receiver;
	UVC_VoiceBroadcast* VoiceBroadcast;

	FVC_Settings Settings;
	bool InitStatus;

	// TODO: to Constructor
	int AllChannels = 1;
};
