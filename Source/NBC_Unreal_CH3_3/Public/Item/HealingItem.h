#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class NBC_UNREAL_CH3_3_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AHealingItem();

	// 힐량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealAmount;

	virtual void ActivateItem(AActor* Activator) override;
};
