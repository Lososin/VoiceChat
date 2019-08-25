#pragma once

#include "CoreMinimal.h"
#include "VCSender.h"
#include "VCClient.h"
#include "VCServer.h"
#include "VCSourceInfo.h"
#include "VCFunctionLibrary.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVCFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	static AVCClient* CreateVoiceChatClientWithSettings(FVCSettings Settings);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	static AVCClient* CreateVoiceChatClient();

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	static AVCServer* CreateVoiceChatServerWithSettings(FVCSettings Settings);

	UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	static AVCServer* CreateVoiceChatServer();

	UFUNCTION(BlueprintPure, Category = "VoiceChat")
	static FVCSettings GetVoiceChatSettingsFromEngineINI();
};