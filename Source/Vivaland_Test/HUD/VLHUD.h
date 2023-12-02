// HiveFive_Vivaland Test

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD/UserWidget/VLUserWidget.h"
#include "VLHUD.generated.h"

/**
 * 
 */
UCLASS()
class VIVALAND_TEST_API AVLHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	TSubclassOf<UUserWidget> PlayerScoreWidgetClass;

	UPROPERTY()
	UVLUserWidget* PlayerScoreUserWidget;


protected:

	virtual void BeginPlay() override;
	void AddPlayerScoreWidget();
};
