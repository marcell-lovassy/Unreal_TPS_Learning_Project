// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Engine/DamageEvents.h"
#include "ShooterCharacter.h"
#include <AIController.h>

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}


// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, FName("MuzzleFlashSocket"));
	
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (ownerPawn == nullptr) return;

	AController* ownerController = ownerPawn->GetController();
	bool isAI = Cast<AAIController>(ownerController) != nullptr;

	AShooterCharacter* shooter = Cast<AShooterCharacter>(ownerPawn);

	if (ownerController == nullptr) return;

	FVector viewPointLocation;
	FRotator viewPointRotation;

	ownerController->GetPlayerViewPoint(viewPointLocation, viewPointRotation);
	FVector endPoint = viewPointLocation + viewPointRotation.Vector() * MaxRange;

	FHitResult hitResult;

	FCollisionQueryParams collisionParams;

	collisionParams.AddIgnoredActor(this);
	collisionParams.AddIgnoredActor(GetOwner());
	
	bool isHit = GetWorld()->LineTraceSingleByChannel(hitResult, viewPointLocation + viewPointRotation.Vector() * (isAI ? 1 : shooter->GetCameraDistance()), endPoint, ECollisionChannel::ECC_GameTraceChannel1, collisionParams);
	if (isHit)
	{
		FVector shotDirection = -viewPointRotation.Vector();
		FPointDamageEvent DamageEvent = FPointDamageEvent(Damage, hitResult, shotDirection, nullptr);

		AActor* hitActor = hitResult.GetActor();
		if(hitActor && hitActor != ownerPawn)
		{
			ACharacter* character = Cast<ACharacter>(hitActor);
			UParticleSystem* particlesToSpawn;
			particlesToSpawn = character ? ImpactEffectCharacter : ImpactEffectWorld;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particlesToSpawn, hitResult.ImpactPoint, shotDirection.Rotation());
			
			hitActor->TakeDamage(Damage, DamageEvent, ownerController, this);
		}
		//DrawDebugPoint(GetWorld(), viewPointLocation + viewPointRotation.Vector() * (isAI ? 1 : shooter->GetCameraDistance()), 20, FColor::Red, true);
	}
	//DrawDebugCamera(GetWorld(), viewPointLocation, viewPointRotation, 90, 2.f, FColor::Red, true);
}