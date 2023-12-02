// HiveFive_Vivaland Test

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VLProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class USoundCue;
class UBoxComponent;

UCLASS()
class VIVALAND_TEST_API AVLProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AVLProjectile();

	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

	/**
	 * @brief - Projectile Hit Event
	 * @param HitComp - Component that causes the hit (our projectile)
	 * @param OtherActor -the actor that was hit
	 * @param OtherComp - the other comp that was hit
	 * @param NormalImpulse - the impulse, kinda perpendicular
	 * @param Hit - FHitResult with the storaged infos
	 */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	virtual void Destroyed() override; 

	// UFUNCTION(Server, Reliable)
	// void ServerDestroyed();
	// UFUNCTION(NetMulticast, Reliable)
	// void MulticastDestroyed();
	
private:

	// Projectile CollisionBox and ProjectileMovementComponent, set in blueprints.
	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	
	// Tracer and component, to be spawned
	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;
	UPROPERTY()
	UParticleSystemComponent* TracerComponent;
	// impact FX's to be spawned when projectile hits something.
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;
	UPROPERTY(EditAnywhere)
	USoundCue* ImpactSound;
	
};
