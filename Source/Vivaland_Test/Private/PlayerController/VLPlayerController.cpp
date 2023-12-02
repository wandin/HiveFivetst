// HiveFive_Vivaland Test


#include "PlayerController/VLPlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/VLCharacter.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/VLPlayerState.h"
#include "Vivaland_Test/HUD/VLHUD.h"

class UPawnAction_Move;

AVLPlayerController::AVLPlayerController()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Custom;
	CachedDestination = FVector::ZeroVector;
	NetUpdateFrequency = 120.f;
	MinNetUpdateFrequency = 60.f;
}

void AVLPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	HUD = Cast<AVLHUD>(GetHUD());
}

void AVLPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	PollInit();
}

void AVLPlayerController::SetHUDScore(float Score)
{
	HUD = HUD == nullptr ? Cast<AVLHUD>(GetHUD()) : HUD;

	bool bHUDValid = HUD && HUD->PlayerScoreUserWidget && HUD->PlayerScoreUserWidget->Score;
	if(bHUDValid)
	{
		MYPlayerState = Cast<AVLPlayerState>(GetPlayerState<AVLPlayerState>());

		if(MYPlayerState)
		{
			const FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
			HUD->PlayerScoreUserWidget->Score->SetText(FText::FromString(ScoreText));
		}
	}
}

void AVLPlayerController::SetHUDEnemyScore(int32 EnemyScore)
{
	HUD = HUD == nullptr ? Cast<AVLHUD>(GetHUD()) : HUD;
	bool bHUDValid = HUD &&
		HUD->PlayerScoreUserWidget &&
		HUD->PlayerScoreUserWidget->EnemyScore;
	if (bHUDValid)
	{
		MYPlayerState = Cast<AVLPlayerState>(GetPlayerState<AVLPlayerState>());

		if(MYPlayerState)
		{
			const FString EnemyScoreText = FString::Printf(TEXT("%d"), EnemyScore);
			HUD->PlayerScoreUserWidget->EnemyScore->SetText(FText::FromString(EnemyScoreText));
		}
	}
}

void AVLPlayerController::PollInit()
{
	if (MYPlayerState)
	{
		MYPlayerState->AddToScore(0.f);
		MYPlayerState->AddToDefeats(0);
	}
	if (MYPlayerState == nullptr)
	{
		MYPlayerState = GetPlayerState<AVLPlayerState>();
		if (MYPlayerState)
		{
			MYPlayerState->AddToScore(0.f);
			MYPlayerState->AddToDefeats(0);
		}
	}
}

void AVLPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVLPlayerController, CachedDestination);
}

void AVLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVLPlayerController::ClickTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AVLPlayerController::ClickReleased);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &AVLPlayerController::ClickReleased);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered,  this, &AVLPlayerController::FirePressed);
	}
}

void AVLPlayerController::ClickTriggered()
{
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	const bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	
	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	// Move towards mouse pointer or touch
	const APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();

		if(GetLocalRole() < ROLE_Authority)
		{
			ServerClickTriggered(WorldDirection);
		}
		MulticastClickTriggered(CachedDestination);
	}
}
void AVLPlayerController::ClickReleased()
{
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerClickReleased(CachedDestination);
	}
	MulticastClickReleased(CachedDestination);
}

void AVLPlayerController::ServerClickReleased_Implementation(FVector Destination)
{
	if(HasAuthority())
	{
		MulticastClickReleased(Destination);
	}
}

void AVLPlayerController::MulticastClickReleased_Implementation(FVector Destination)
{
	// We move there and spawn the click particles
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
}

void AVLPlayerController::ServerClickTriggered_Implementation(FVector Destination)
{
	if(HasAuthority())
	{
		MulticastClickTriggered(Destination);
	}
}

void AVLPlayerController::MulticastClickTriggered_Implementation(FVector Destination)
{
	// Move towards mouse pointer
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		ControlledPawn->AddMovementInput(Destination, 1.0, false);
	}
}

void AVLPlayerController::FirePressed()
{
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerFirePressed();
		return;
	}
	MulticastFirePressed();
}

void AVLPlayerController::ServerFirePressed_Implementation()
{
	if(HasAuthority())
	{
		MulticastFirePressed();
	}
}

void AVLPlayerController::MulticastFirePressed_Implementation()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (AVLCharacter* OwningCharacter = Cast<AVLCharacter>(ControlledPawn))
		{
			OwningCharacter->Fire();
		}
	}
}