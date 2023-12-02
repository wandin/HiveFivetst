// HiveFive_Vivaland Test

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerController/VLPlayerController.h"
#include "VLCharacter.generated.h"

UCLASS()
class VIVALAND_TEST_API AVLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVLCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
	
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	void Fire();
	void Eliminate();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastEliminate();

protected:
	
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	/* Mesh */
	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* TankBaseMesh;
	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* TankBarrelMesh;
	/* PROJECTILE*/
	UPROPERTY(EditAnywhere, Category = Projectile)
	USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AVLProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TObjectPtr<AVLProjectile> Projectile = nullptr;

	UPROPERTY(EditAnywhere)
	UParticleSystem* DeathParticles;
	UParticleSystemComponent* ParticlesComponent;
	
	UPROPERTY()
	AVLPlayerController* PlayerController;


	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	void ElimTimerFinished();
};
