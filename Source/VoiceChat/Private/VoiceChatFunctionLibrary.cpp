#pragma once

#include "VoiceChatFunctionLibrary.h"

AVoiceChatClient* UVoiceChatFunctionLibrary::CreateVoiceChatClientWithSettings(FVoiceChatSettings Settings) {
	auto temp = NewObject<AVoiceChatClient>();
	temp->ConfigureVoiceChat(Settings);
	return temp;
}

AVoiceChatClient* UVoiceChatFunctionLibrary::CreateVoiceChatClient() {
	return NewObject<AVoiceChatClient>();
}

AVoiceChatServer* UVoiceChatFunctionLibrary::CreateVoiceChatServerWithSettings(FVoiceChatSettings Settings) {
	auto temp = NewObject<AVoiceChatServer>();
	temp->ConfigureVoiceChat(Settings);
	return temp;
}

AVoiceChatServer* UVoiceChatFunctionLibrary::CreateVoiceChatServer() {
	return NewObject<AVoiceChatServer>();
}

FVoiceChatSettings UVoiceChatFunctionLibrary::GetVoiceChatSettingsFromEngineINI() {
	FVoiceChatSettings Settings;

	GConfig->GetInt(TEXT("VoiceChatPlugin"), TEXT("SampleRate"), Settings.SampleRate, GEngineIni);
	if (Settings.SampleRate == 0)
		UE_LOG(VoiceChatLog, Warning, TEXT("SampleRate not found (DefaultEngine.ini)"));

	GConfig->GetInt(TEXT("VoiceChatPlugin"), TEXT("ServerPort"), Settings.ServerPort, GEngineIni);
	if (Settings.ServerPort == 0)
		UE_LOG(VoiceChatLog, Warning, TEXT("ServerPort not found (DefaultEngine.ini)"));

	GConfig->GetInt(TEXT("VoiceChatPlugin"), TEXT("ClientPort"), Settings.ClientPort, GEngineIni);
	if (Settings.ClientPort == 0)
		UE_LOG(VoiceChatLog, Warning, TEXT("ClientPort not found (DefaultEngine.ini)"));

	GConfig->GetInt(TEXT("VoiceChatPlugin"), TEXT("BufferSize"), Settings.BufferSize, GEngineIni);
	if (Settings.BufferSize == 0)
		UE_LOG(VoiceChatLog, Warning, TEXT("BufferSize not found (DefaultEngine.ini)"));
	
	FText ip;
	GConfig->GetText(TEXT("VoiceChatPlugin"), TEXT("ClientPort"), ip, GEngineIni);
	if (ip.IsEmpty()) {
		UE_LOG(VoiceChatLog, Warning, TEXT("ServerIP not found (DefaultEngine.ini)"));
	}

	Settings.ServerIP = ip.ToString();

	return Settings;
}