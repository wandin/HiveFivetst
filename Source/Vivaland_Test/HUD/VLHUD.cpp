// HiveFive_Vivaland Test


#include "VLHUD.h"

void AVLHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AVLHUD::BeginPlay()
{
	Super::BeginPlay();

	AddPlayerScoreWidget();
}

void AVLHUD::AddPlayerScoreWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && PlayerScoreWidgetClass)
	{
		PlayerScoreUserWidget = CreateWidget<UVLUserWidget>(PlayerController, PlayerScoreWidgetClass);
		PlayerScoreUserWidget->AddToViewport();
	}
}
