// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

void AKillEmAllGameMode::PawnKilled(APawn* killedPawn)
{
	Super::PawnKilled(killedPawn);

	APlayerController* playerController = Cast<APlayerController>(killedPawn->GetController());

	if (playerController)
	{
		playerController->GameHasEnded(nullptr, false);
	}

	UE_LOG(LogTemp, Warning, TEXT("Pawn % s killed"), *killedPawn->GetName());
}