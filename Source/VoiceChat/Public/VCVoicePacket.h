#pragma once

#include "CoreMinimal.h"
#include "VCVoicePacket.generated.h"

USTRUCT(BlueprintType)
struct FVCVoicePacket {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	FString Meta1 = "Meta";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	FString Meta2 = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	TArray<uint8> VoiceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int ChannelsNum = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int CurrentChannel = 0;

	FVoiceChatData() {}
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FVoiceChatData& TheStruct) {
	Ar << TheStruct.Meta1;
	Ar << TheStruct.Meta2;
	Ar << TheStruct.Data;
	Ar << TheStruct.ChannelsNum;
	Ar << TheStruct.CurrentChannel;

	return Ar;
}