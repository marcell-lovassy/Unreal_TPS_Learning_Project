// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = "Update Player Location if Seen";
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (playerPawn == nullptr) return;

	if (AAIController* controller = OwnerComp.GetAIOwner())
	{
		if (controller->LineOfSightTo(playerPawn))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), playerPawn->GetActorLocation());
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		}
	}
}
