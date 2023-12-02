// Fill out your copyright notice in the Description page of Project Settings.


#include "Vivaland_Test/Public/Pawn/VLPawn.h"

#include "AITypes.h"
#include "Actions/PawnAction_Move.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AVLPawn::AVLPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	/* Don't rotate character to camera direction */
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	/* Capsule comp */
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCollisionObjectType(ECC_Pawn);
	CapsuleComponent->InitCapsuleSize(300.f,300.f);
	RootComponent = CapsuleComponent;
	
	/* Movement Comp */
	MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("MovementComp"));
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0.f, 640.f, 0.f);
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->bSnapToPlaneAtStart = true;

	/* Spring Arm comp */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(CapsuleComponent);
	SpringArm->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	SpringArm->TargetArmLength = 1400.f;
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->bDoCollisionTest = false;
	
	/*	Camera */
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	/* Tank Mesh */
	TankBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	TankBaseMesh->SetupAttachment(CapsuleComponent);
	TankBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	TankBarrelMesh->SetupAttachment(TankBaseMesh);
	
	/* Projectile Spawn spot */
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TankBarrelMesh);
}

void AVLPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}