#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ch3PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class NBC_UNREAL_CH3_3_API ACh3PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACh3PlayerController();

	// IMC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input");
	UInputMappingContext* InputMappingContext;
	// IA_Move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input");
	UInputAction* MoveAction;
	// IA_Jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input");
	UInputAction* JumpAction;
	// IA_Look
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input");
	UInputAction* LookAction;
	// IA_Sprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input");
	UInputAction* SprintAction;

	virtual void BeginPlay() override;
};
