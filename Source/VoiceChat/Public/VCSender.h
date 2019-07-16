#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPv4Address.h"
#include "VCSender.generated.h"

//TODO: To class with initialization in constructor
USTRUCT(BlueprintType)
class FVCSender {
	GENERATED_USTRUCT_BODY()
	
    TSharedPtr<FInternetAddr> RemoteAddress;

	FSocket* SenderSocket;

	int Channel = 0;

	int LifeTime = 100'000; //100 secs
};