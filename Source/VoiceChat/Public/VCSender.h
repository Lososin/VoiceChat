#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPv4Address.h"
#include "VCSender.generated.h"

USTRUCT(BlueprintType)
struct FVCSender {
	GENERATED_USTRUCT_BODY()
	
    TSharedPtr<FInternetAddr> RemoteAddress;

	FSocket* SenderSocket;

	int Channel = 0;

	int LifeTime = 100000; //100 secs
};