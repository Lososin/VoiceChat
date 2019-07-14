#pragma once

#include "CoreMinimal.h"
#include "VoiceChatClientInfo.h"
#include "Networking.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPv4Address.h"
#include "VoiceChatServerInfo.generated.h"

USTRUCT(BlueprintType)
struct FVoiceChatServerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	FVoiceChatClientInfo ClientInfo;

    TSharedPtr<FInternetAddr> RemoteAddress;

	FSocket* SenderSocket;
};