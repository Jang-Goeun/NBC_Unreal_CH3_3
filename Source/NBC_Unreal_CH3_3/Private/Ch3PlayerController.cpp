#include "Ch3PlayerController.h"

#include "Ch3GameInstance.h"
#include "Ch3GameState.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ACh3PlayerController::ACh3PlayerController()
	: InputMappingContext(nullptr),
      MoveAction(nullptr),
	  JumpAction(nullptr),
	  LookAction(nullptr),
	  SprintAction(nullptr),
	  HUDWidgetClass(nullptr),
	  HUDWidgetInstance(nullptr),
	  MianMenuidgetClass(nullptr),
	  MianMenuWidgetInstance(nullptr)
{
}

void ACh3PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* ACh3PlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}


void ACh3PlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MianMenuWidgetInstance)
	{
		MianMenuWidgetInstance->RemoveFromParent();
		MianMenuWidgetInstance = nullptr;
	}

	if (MianMenuidgetClass)
	{
		MianMenuWidgetInstance = CreateWidget<UUserWidget>(this, MianMenuidgetClass);
		if (MianMenuWidgetInstance)
		{
			MianMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MianMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			ButtonText->SetText(FText::FromString(bIsRestart ? TEXT("ReStart") : TEXT("Start")));
		}

		UWidget* GameOverText = MianMenuWidgetInstance->GetWidgetFromName(TEXT("GameOverText"));
		UTextBlock* TotalScoreText = Cast<UTextBlock>(MianMenuWidgetInstance->GetWidgetFromName(TEXT("TotalScoreText")));

		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MianMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MianMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (GameOverText)
			{
				GameOverText->SetVisibility(ESlateVisibility::Visible);
			}

			if (TotalScoreText)
			{
				if (UCh3GameInstance* Ch3GameInstance = Cast<UCh3GameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: %d"), Ch3GameInstance->TotalScore)
					));
				}
				TotalScoreText->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			if (GameOverText)
			{
				GameOverText->SetVisibility(ESlateVisibility::Collapsed);
			}
			if (TotalScoreText)
			{
				TotalScoreText->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void ACh3PlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MianMenuWidgetInstance)
	{
		MianMenuWidgetInstance->RemoveFromParent();
		MianMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}

		ACh3GameState* Ch3GameState = GetWorld() ? GetWorld()->GetGameState<ACh3GameState>() : nullptr;

		if (Ch3GameState)
		{
			Ch3GameState->UpdateHUD();
		}
	}
}

void ACh3PlayerController::StartGame()
{
	if (UCh3GameInstance* Ch3GameInstance = Cast<UCh3GameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		Ch3GameInstance->CurrentLevelIndex = 0;
		Ch3GameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
}
