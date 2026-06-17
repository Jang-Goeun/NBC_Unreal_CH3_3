#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Ch3GameInstance.generated.h"

UCLASS()
class NBC_UNREAL_CH3_3_API UCh3GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCh3GameInstance();

	// 게임 전체 누적 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	// 현재 레벨 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
};
