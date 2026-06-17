#include "Ch3GameInstance.h"

UCh3GameInstance::UCh3GameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UCh3GameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}
