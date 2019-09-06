#pragma once

#include "CoreMinimal.h"
#include "VC_Settings.h"
#include "VC_FunctionLibrary.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVC_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	// UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	// static AVCClient* CreateVoiceChatClientWithSettings(FVCSettings Settings);

	// UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	// static AVCClient* CreateVoiceChatClient();

	// UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	// static AVCServer* CreateVoiceChatServerWithSettings(FVCSettings Settings);

	// UFUNCTION(BlueprintCallable, Category = "VoiceChat")
	// static AVCServer* CreateVoiceChatServer();

	UFUNCTION(BlueprintPure, Category = "VoiceChat, ClientServer Initialization")
	static FVC_Settings GetVoiceChatSettingsFromEngineINI();
};