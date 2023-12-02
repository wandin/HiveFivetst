// HiveFive_Vivaland Test

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VLUserWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class VIVALAND_TEST_API UVLUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Score;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyScore;

	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* PlayerName;
};
