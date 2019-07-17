#pragma once

#include "CoreMinimal.h"
#include "VoiceOver.h"
#include "VoiceChatClient.h"
#include "VoiceChatServer.h"
#include "VoiceChatFunctionLibrary.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVoiceChatFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	static AVoiceChatClient* CreateVoiceChatClientWithSettings(FVoiceChatSettings Settings);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	static AVoiceChatClient* CreateVoiceChatClient();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	static AVoiceChatServer* CreateVoiceChatServerWithSettings(FVoiceChatSettings Settings);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	static AVoiceChatServer* CreateVoiceChatServer();

	UFUNCTION(BlueprintPure, Category = "VoiceChat")
	static FVoiceChatSettings GetVoiceChatSettingsFromEngineINI();
};