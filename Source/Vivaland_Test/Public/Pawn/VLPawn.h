// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VLPawn.generated.h"


class UCharacterMovementComponent;

UCLASS()
class VIVALAND_TEST_API AVLPawn : public APawn
{
	GENERATED_BODY()

public:
	AVLPawn();

	virtual void Tick(float DeltaSeconds) override;
	
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArm; }

private:

	/* Capsule and movement components */
	UPROPERTY(EditAnywhere, Category = Movement)
	class UCapsuleComponent* CapsuleComponent;
	/**/
	UPROPERTY(EditAnywhere, Category = Movement)
	UCharacterMovementComponent* MovementComponent;
	
	/* Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;
	/* Spring Arm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	/* Mesh */
	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* TankBaseMesh;
	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* TankBarrelMesh;
	UPROPERTY(EditAnywhere, Category = Projectile)
	USceneComponent* ProjectileSpawnPoint;
};
