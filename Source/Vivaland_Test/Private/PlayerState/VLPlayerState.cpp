// HiveFive_Vivaland Test


#include "PlayerState/VLPlayerState.h"

#include "Character/VLCharacter.h"
#include "Net/UnrealNetwork.h"

void AVLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AVLPlayerState, Defeats);
}

void AVLPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);
	Character = Character == nullptr ? Cast<AVLCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller == nullptr ? Cast<AVLPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}
void AVLPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	
	Character = Character == nullptr ? Cast<AVLCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller == nullptr ? Cast<AVLPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void AVLPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	Character = Character == nullptr ? Cast<AVLCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller == nullptr ? Cast<AVLPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDEnemyScore(Defeats);
		}
	}
}

void AVLPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<AVLCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller == nullptr ? Cast<AVLPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDEnemyScore(Defeats);
		}
	}
}