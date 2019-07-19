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

	FVCVoicePacket() {}

	FVCVoicePacket(FString _Meta1, FString _Meta2, TArray<uint8> _VoiceData) : 
			Meta1(_Meta1), Meta2(_Meta2), VoiceData(_VoiceData) {}
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FVCVoicePacket& Packet) {
	Ar << Packet.Meta1;
	Ar << Packet.Meta2;
	Ar << Packet.VoiceData;
	Ar << Packet.ChannelsNum;
	Ar << Packet.CurrentChannel;

	return Ar;
}