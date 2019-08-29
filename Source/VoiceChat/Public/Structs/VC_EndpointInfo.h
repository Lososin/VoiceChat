#pragma once

#include "CoreMinimal.h"
#include "VC_EndpointInfo.generated.h"

USTRUCT(BlueprintType)
struct FVC_EndpointInfo {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	FString IP = "127.0.0.1";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int Port = 9999;

	FVC_EndpointInfo() {}

	FVC_EndpointInfo(FString _IP, int _Port) : IP(_IP), Port(_Port) {}
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FVC_EndpointInfo& TheStruct) {
	Ar << TheStruct.IP;
	Ar << TheStruct.Port;

	return Ar;
}