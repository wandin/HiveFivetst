// HiveFive_Vivaland Test

#pragma once

#include "CoreMinimal.h"
#include "Character/VLCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "VLGameModeBase.generated.h"

class AVLPlayerController;
/**
 * 
 */
UCLASS()
class VIVALAND_TEST_API AVLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVLGameModeBase();

	virtual void PlayerEliminated(class AVLCharacter* ElimmedCharacter, class AVLPlayerController* VictimController, AVLPlayerController* AttackerController);
	void RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController);
};
