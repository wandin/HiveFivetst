// HiveFive_Vivaland Test

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "VLPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class VIVALAND_TEST_API AVLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);
	
	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;

private:
	UPROPERTY()
	class AVLCharacter* Character;
	UPROPERTY()
	class AVLPlayerController* Controller;

protected:
	/* Replication notifies	*/
	virtual void OnRep_Score() override;

	UFUNCTION()
	virtual void OnRep_Defeats();
};
