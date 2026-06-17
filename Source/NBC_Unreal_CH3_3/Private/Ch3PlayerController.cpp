#include "Ch3PlayerController.h"

#include "Ch3GameInstance.h"
#include "Ch3GameState.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ACh3PlayerController::ACh3PlayerController()
	: InputMappingContext(nullptr),
      MoveAction(nullptr),
	  JumpAction(nullptr),
	  LookAction(nullptr),
	  SprintAction(nullptr),
	  HUDWidgetClass(nullptr),
	  HUDWidgetInstance(nullptr),
	  MainMenuidgetClass(nullptr),
	  MainMenuWidgetInstance(nullptr)
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

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (MainMenuidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			ButtonText->SetText(FText::FromString(bIsRestart ? TEXT("ReStart") : TEXT("Start")));
		}

		UWidget* GameOverText = MainMenuWidgetInstance->GetWidgetFromName(TEXT("GameOverText"));
		UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TotalScoreText")));

		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
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

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
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

void ACh3PlayerController::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}

void ACh3PlayerController::BackToMainMenu()
{
	SetPause(false);
	UGameplayStatics::OpenLevel(GetWorld(), FName("MenuLevel"));
}
