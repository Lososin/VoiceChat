#pragma once

#include "VC_FunctionLibrary.h"

// AVCClient* UVCFunctionLibrary::CreateVoiceChatClientWithSettings(FVCSettings Settings) {
// 	auto temp = NewObject<AVCClient>();
// 	temp->SetSettings(Settings);
// 	return temp;
// };

// AVCClient* UVCFunctionLibrary::CreateVoiceChatClient() {
// 	return NewObject<AVCClient>();
// };

// AVCServer* UVCFunctionLibrary::CreateVoiceChatServerWithSettings(FVCSettings Settings) {
// 	auto temp = NewObject<AVCServer>();
// 	temp->SetSettings(Settings);
// 	return temp;
// };

// AVCServer* UVCFunctionLibrary::CreateVoiceChatServer() {
// 	return NewObject<AVCServer>();
// };

FVC_Settings UVC_FunctionLibrary::GetVoiceChatSettingsFromEngineINI() {
	FVC_Settings Settings;

	// TODO: Check valid

	if (!GConfig->GetInt(TEXT("VoiceChatPlugin"), TEXT("SampleRate"), Settings.SampleRate, GEngineIni)) {
		UE_LOG(VoiceChatLog, Warning, TEXT("SampleRate not found (DefaultEngine.ini). Default is 44100"));
        Settings.SampleRate = 44100;
    }

	if (!GConfig->GetInt(TEXT("VoiceChatPlugin"), TEXT("ServerPort"), Settings.ServerPort, GEngineIni)) {
		UE_LOG(VoiceChatLog, Warning, TEXT("ServerPort not found (DefaultEngine.ini). Default is 8001"));
        Settings.ServerPort = 8001;
    }

	if (!GConfig->GetInt(TEXT("VoiceChatPlugin"), TEXT("ClientPort"), Settings.ClientPort, GEngineIni)) {
		UE_LOG(VoiceChatLog, Warning, TEXT("ClientPort not found (DefaultEngine.ini). Default is 8002"));
        Settings.ClientPort = 8002;
    }

	if (!GConfig->GetInt(TEXT("VoiceChatPlugin"), TEXT("BufferSize"), Settings.BufferSize, GEngineIni)) {
		UE_LOG(VoiceChatLog, Warning, TEXT("BufferSize not found (DefaultEngine.ini). Default is 1048510"));
        Settings.BufferSize = 1048510;
    }
	
	FText ip;
	GConfig->GetText(TEXT("VoiceChatPlugin"), TEXT("ClientPort"), ip, GEngineIni);
	if (ip.IsEmpty()) {
		UE_LOG(VoiceChatLog, Warning, TEXT("ServerIP not found (DefaultEngine.ini). Default is 127.0.0.1"));
        ip = FText::FromString("127.0.0.1");
	}

	Settings.ServerIP = ip.ToString();

	return Settings;
};