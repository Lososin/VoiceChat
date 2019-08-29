#pragma once

#include "CoreMinimal.h"
#include "VC_Address.generated.h"

USTRUCT(BlueprintType)
struct FVC_Address {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	FString IP = "127.0.0.1";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int Port = 9999;

	FVC_Address() {}

	FVC_Address(FString _IP, int _Port) : IP(_IP), Port(_Port) {}
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FVC_Address& TheStruct) {
	Ar << TheStruct.IP;
	Ar << TheStruct.Port;

	return Ar;
}