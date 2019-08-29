#pragma once

#include "CoreMinimal.h"
#include "VC_Packet.generated.h"

USTRUCT(BlueprintType)
struct FVC_Packet {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	FString Meta = "Meta";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	TArray<uint8> VoiceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoiceChat")
	int Channel = -1;

	FVC_Packet() {}

	FVC_Packet(FString _Meta, TArray<uint8> _VoiceData) : 
			Meta(_Meta), VoiceData(_VoiceData) {}
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FVC_Packet& Packet) {
	Ar << Packet.Meta;
	Ar << Packet.VoiceData;
	Ar << Packet.Channel;

	return Ar;
}