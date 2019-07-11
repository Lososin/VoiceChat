#pragma once

#include "CoreMinimal.h"
#include "VoiceChatSettings.h"
#include "VoiceChatServer.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVoiceChatServer : public UObject
{
	GENERATED_BODY()

public:
	UVoiceChatServer();
	~UVoiceChatServer();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	void ConfigureVoiceChat(FVoiceChatSettings set);

	void InitVoiceChat();

	UPROPERTY(BlueprintReadOnly)
	FVoiceChatSettings Settings;
};
