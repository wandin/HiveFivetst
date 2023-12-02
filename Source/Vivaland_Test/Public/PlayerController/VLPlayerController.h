// HiveFive_Vivaland Test

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "VLPlayerController.generated.h"

class UInputAction;
class UNiagaraSystem;
class AVLPlayerState;

/**
 * 
 */
UCLASS()
class VIVALAND_TEST_API AVLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AVLPlayerController();

	virtual void Tick(float DeltaSeconds) override;
	
	void SetHUDScore(float Score);
	void SetHUDEnemyScore(int32 EnemyScore);
	// Poll for any relevant classes and initialize our HUD
	void PollInit();
	
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext; // ReSharper disable once UnrealHeaderToolError, Disabling Unreal's bullshit -.-
	
	/** InputAction for moving pawn when click */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	/** Input handlers for Move action. */
	void ClickTriggered();
	void ClickReleased();

	void FirePressed();
	
	//Move
	UFUNCTION(Server, Reliable)
	void ServerClickTriggered(FVector Destination);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastClickTriggered(FVector Destination);

	UFUNCTION(Server, Reliable)
	void ServerClickReleased(FVector Destination);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastClickReleased(FVector Destination);

	UFUNCTION(Server, Reliable)
	void ServerFirePressed();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFirePressed();

private:
	UPROPERTY(Replicated)
	FVector CachedDestination;
	
	UPROPERTY()
	class AVLHUD* HUD;

protected:
	UPROPERTY()
	AVLPlayerState* MYPlayerState;
};
