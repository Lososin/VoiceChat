#pragma once

#include "CoreMinimal.h"
#include "VoiceCapture.h"
#include "VoiceChatCapture.generated.h"

UCLASS(ClassGroup = VoiceChat, Blueprintable)
class UVoiceChatCapture : public UObject
{
	GENERATED_BODY()

public:
	UVoiceChatCapture();
	~UVoiceChatCapture();
};
