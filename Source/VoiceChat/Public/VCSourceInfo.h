#pragma once

#include "CoreMinimal.h"
#include "VCSourceInfo.generated.h"

USTRUCT(BlueprintType)
struct FVCSourceInfo {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	FString IP = "127.0.0.1";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int Port = 9999;

	FVCSourceInfo() {}
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FVCSourceInfo& TheStruct) {
	Ar << TheStruct.IP;
	Ar << TheStruct.Port;

	return Ar;
}