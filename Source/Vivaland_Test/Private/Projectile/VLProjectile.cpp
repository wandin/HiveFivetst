// HiveFive_Vivaland Test


#include "Projectile/VLProjectile.h"

#include "Character/VLCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"


AVLProjectile::AVLProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	// Collision Box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Adicione o canal personalizado para o projetil
	CollisionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	// Configure para ignorar o canal do proprietário
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // Substitua ECC_Pawn pelo canal do proprietário

	// Configure para outros canais conforme necessário
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->MaxSpeed = 5000.f;
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->SetIsReplicated(true);
}

void AVLProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition);
	}

	// binding our OnHit Delegates
	if(HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AVLProjectile::OnHit);
	}
	SetLifeSpan(2.f);
}

void AVLProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVLProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(OwnerCharacter)
	{
		AController* OwnerController = OwnerCharacter->GetController();
		if(OwnerController)
		{
			UGameplayStatics::ApplyDamage(OtherActor, 1.f, OwnerController, this, UDamageType::StaticClass());
		}
	}
	Destroyed();
}

void AVLProjectile::Destroyed()
{
	Super::Destroyed();
	if(ImpactParticles) // Spawn our ImpactParticles
		{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
		}
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
	Destroy();
}