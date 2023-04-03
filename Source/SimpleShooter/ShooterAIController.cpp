// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior) 
	{
		RunBehaviorTree(AIBehavior);

		blackboardComponent = GetBlackboardComponent();


		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		blackboardComponent->SetValueAsVector(FName("StartLocation"), GetPawn()->GetActorLocation());
		blackboardComponent->SetValueAsVector(FName("PlayerLocation"), PlayerPawn->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//this is now all in BTServices
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//if (LineOfSightTo(PlayerPawn)) 
	//{
	//	blackboardComponent->SetValueAsVector(FName("PlayerLocation"), PlayerPawn->GetActorLocation());
	//	//blackboardComponent->SetValueAsVector(FName("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
	//}
	//else 
	//{
	//	blackboardComponent->ClearValue(FName("PlayerLocation"));
	//}


	//Code without behaviour tree
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//if (LineOfSightTo(PlayerPawn)) 
	//{
	//	SetFocus(PlayerPawn);
	//	MoveToActor(PlayerPawn, AcceptanceRadius);
	//}
	//else 
	//{
	//	ClearFocus(EAIFocusPriority::Gameplay);
	//	StopMovement();
	//}
}

bool AShooterAIController::IsDead() const
{
	if(AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn()))
	{
		return ControlledCharacter->IsDead();
	}

	return true;
}
