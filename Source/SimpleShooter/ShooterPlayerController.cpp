// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Hud = CreateWidget(this, HudClass);
	if (Hud)
	{
		Hud->AddToViewport();
	}
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if(bIsWinner)
	{
		if (UUserWidget* WinScreen = CreateWidget(this, WinScreenClass))
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		if(UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass))
		{
			LoseScreen->AddToViewport();
		}
	}
	Hud->RemoveFromParent();
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}