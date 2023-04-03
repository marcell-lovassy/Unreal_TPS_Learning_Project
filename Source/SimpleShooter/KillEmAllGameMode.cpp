// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* KilledPawn)
{
	Super::PawnKilled(KilledPawn);

	APlayerController* playerController = Cast<APlayerController>(KilledPawn->GetController());

	if (playerController)
	{
		EndGame(false);
	}
	else
	{
		for (const AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
		{
			if(!Controller->IsDead())
			{
				return;
			}
		}
		EndGame(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("Pawn %s killed"), *KilledPawn->GetName());
}

void AKillEmAllGameMode::EndGame(bool bPlayerWon)
{
	for(AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bPlayerWon;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
