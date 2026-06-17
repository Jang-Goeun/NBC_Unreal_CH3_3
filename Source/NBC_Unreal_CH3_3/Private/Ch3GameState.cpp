#include "Ch3GameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "Ch3GameInstance.h"
#include "Ch3PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Item/CoinItem.h"

ACh3GameState::ACh3GameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f; // 한 레벨당 30초
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	CurrentWaveIndex = 0;
}

void ACh3GameState::BeginPlay()
{
	Super::BeginPlay();

	FString CurrentMapName = GetWorld()->GetMapName();
	if (!CurrentMapName.Contains("MenuLevel"))
	{
		StartLevel();

		GetWorldTimerManager().SetTimer(
			HUDUpdateTimerHandle,
			this,
			&ACh3GameState::UpdateHUD,
			0.1f,
			true
		);
	}
}

int32 ACh3GameState::GetScore() const
{
	return Score;
}

void ACh3GameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCh3GameInstance* Ch3GameInstance = Cast<UCh3GameInstance>(GameInstance);
		if (Ch3GameInstance)
		{
			Ch3GameInstance->AddToScore(Amount);
		}
	}
}

void ACh3GameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACh3PlayerController* Ch3PlayerController = Cast<ACh3PlayerController>(PlayerController))
		{
			Ch3PlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCh3GameInstance* Ch3GameInstance = Cast<UCh3GameInstance>(GameInstance);
		if (Ch3GameInstance)
		{
			CurrentLevelIndex = Ch3GameInstance->CurrentLevelIndex;
		}
	}

	// 레벨 시작 시, 코인 개수 초기화
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CurrentWaveIndex = 0;
	StartWave();
}

void ACh3GameState::OnLevelTimeUp()
{
	OnWaveTimeUp();
}

void ACh3GameState::OnCoinCollected()
{
	CollectedCoinCount++;

	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount)

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave();
	}
}

void ACh3GameState::EndLevel()
{
	// 타이머 해제
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCh3GameInstance* Ch3GameInstance = Cast<UCh3GameInstance>(GameInstance);
		if (Ch3GameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			Ch3GameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	// 모든 레벨을 다 돌았다면 게임 오버 처리
	if (CurrentLevelIndex >= MaxLevels)
	{
			OnGameOver();
			return;
	}

	// 레벨 맵 이름이 있다면 해당 맵 불러오기
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
			UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
			// 맵 이름이 없으면 게임오버
			OnGameOver();
	}
}

void ACh3GameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACh3PlayerController* Ch3PlayerController = Cast<ACh3PlayerController>(PlayerController))
		{
			Ch3PlayerController->SetPause(true);
			Ch3PlayerController->ShowMainMenu(true);
		}
	}
}

void ACh3GameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACh3PlayerController* Ch3PlayerController = Cast<ACh3PlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = Ch3PlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UCh3GameInstance* Ch3GameInstance = Cast<UCh3GameInstance>(GameInstance);
						if (Ch3GameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Ch3GameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
}

void ACh3GameState::StartWave()
{
	if (WaveData.IsValidIndex(CurrentWaveIndex))
	{
		// [과제 요구사항] 알림 출력
		FString StartMsg = FString::Printf(TEXT("Level %d / Wave %d 시작!"), CurrentLevelIndex + 1, CurrentWaveIndex + 1);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, StartMsg);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *StartMsg);

		// 이번 웨이브의 시간 적용 및 타이머 시작
		float Duration = WaveData[CurrentWaveIndex].WaveDuration;
		GetWorldTimerManager().SetTimer(
			LevelTimerHandle,
			this,
			&ACh3GameState::OnWaveTimeUp,
			Duration,
			false
		);

		// 이번 웨이브의 아이템 개수만큼 스폰 명령
		int32 ItemsToSpawn = WaveData[CurrentWaveIndex].SpawnItemCount;

		TArray<AActor*> FoundVolumes;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

		for (int32 i = 0; i < ItemsToSpawn; i++)
		{
			if (FoundVolumes.Num() > 0)
			{
				ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
				if (SpawnVolume)
				{
					AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
					if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
					{
						SpawnedCoinCount++;
					}
				}
			}
		}
	}
}

void ACh3GameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	CurrentWaveIndex++;

	if (CurrentWaveIndex < WaveData.Num())
	{
		StartWave();
	}
	else
	{
		EndLevel();
	}
}

void ACh3GameState::OnWaveTimeUp()
{
	EndWave();
}
