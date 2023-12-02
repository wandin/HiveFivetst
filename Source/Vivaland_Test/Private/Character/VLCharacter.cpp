// HiveFive_Vivaland Test


#include "Character/VLCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/VLGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/VLProjectile.h"

class AVLPlayerState;
// Sets default values
AVLCharacter::AVLCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(200.f, 200.0f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 2500.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	/* Tank Mesh */
	TankBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	TankBaseMesh->SetupAttachment(GetCapsuleComponent());
	TankBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	TankBarrelMesh->SetupAttachment(TankBaseMesh);
	
	/* Projectile Spawn spot */
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TankBarrelMesh);
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	NetUpdateFrequency = 120.f;
	MinNetUpdateFrequency = 60.f;
}

void AVLCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AVLCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &AVLCharacter::ReceiveDamage);
	}
}

void AVLCharacter::Fire()
{
	if (ProjectileClass)
	{
		const FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		const FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		// Spawn the projectile
		Projectile = GetWorld()->SpawnActor<AVLProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		if(Projectile)
		{
			Projectile->SetOwner(this);
			Projectile->SetInstigator(this);
		}
	}
}

void AVLCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	AVLGameModeBase* VlGameMode = GetWorld()->GetAuthGameMode<AVLGameModeBase>();
	if(VlGameMode)
	{
		PlayerController = (PlayerController == nullptr) ? Cast<AVLPlayerController>(Controller) : PlayerController;
		AVLPlayerController* AttackerController = Cast<AVLPlayerController>(InstigatorController);
		VlGameMode->PlayerEliminated(this, PlayerController, AttackerController);
	}
}

void AVLCharacter::Eliminate()
{
	MulticastEliminate();
	GetWorldTimerManager().SetTimer(ElimTimer,this,&AVLCharacter::ElimTimerFinished,ElimDelay);
}

void AVLCharacter::ElimTimerFinished()
{
	AVLGameModeBase* BlasterGameMode = GetWorld()->GetAuthGameMode<AVLGameModeBase>();
	if (BlasterGameMode)
	{
		BlasterGameMode->RequestRespawn(this, Controller);
	}
}

void AVLCharacter::MulticastEliminate_Implementation()
{
	if(DeathParticles)
	{
		ParticlesComponent = UGameplayStatics::SpawnEmitterAttached(
		DeathParticles,
		GetCapsuleComponent(),
		FName(),
		GetActorLocation(),
		GetActorRotation(),
		EAttachLocation::KeepWorldPosition
	);
	}
}