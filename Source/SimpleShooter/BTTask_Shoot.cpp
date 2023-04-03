// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot() 
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EBTNodeResult::Type result = EBTNodeResult::Failed;

	if (AAIController* controller = OwnerComp.GetAIOwner())
	{
		AShooterCharacter* shooter = Cast<AShooterCharacter>(controller->GetPawn());
		if (shooter) 
		{
			shooter->Shoot();
			result = EBTNodeResult::Succeeded;
		}
	}
	return result;
}