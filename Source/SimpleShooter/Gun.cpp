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

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	AShooterCharacter* Shooter;
	AController* OwnerController = GetOwnerController(Shooter);

	if (OwnerController == nullptr) return false;
	bool isAI = Cast<AAIController>(OwnerController) != nullptr;

	FVector viewPointLocation;
	FRotator viewPointRotation;

	OwnerController->GetPlayerViewPoint(viewPointLocation, viewPointRotation);
	FVector endPoint = viewPointLocation + viewPointRotation.Vector() * MaxRange;

	FCollisionQueryParams collisionParams;

	collisionParams.AddIgnoredActor(this);
	collisionParams.AddIgnoredActor(GetOwner());

	ShotDirection = -viewPointRotation.Vector();

	return GetWorld()->LineTraceSingleByChannel(HitResult, viewPointLocation + viewPointRotation.Vector() * (isAI ? 1 : Shooter->GetCameraDistance()), endPoint, ECollisionChannel::ECC_GameTraceChannel1, collisionParams);
}

AController* AGun::GetOwnerController(AShooterCharacter*& Shooter) const
{
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (ownerPawn == nullptr) return nullptr;

	Shooter = Cast<AShooterCharacter>(ownerPawn);
	AController* ownerController = ownerPawn->GetController();

	return ownerController;
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, FName("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, FName("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDirection;

	if (GunTrace(HitResult, ShotDirection))
	{
		AShooterCharacter* Shooter;
		AController* OwnerController = GetOwnerController(Shooter);
		FPointDamageEvent DamageEvent = FPointDamageEvent(Damage, HitResult, ShotDirection, nullptr);

		AActor* hitActor = HitResult.GetActor();
		if(hitActor)
		{
			ACharacter* character = Cast<ACharacter>(hitActor);
			if (!character) 
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);
			}
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), character ? ImpactEffectCharacter : ImpactEffectWorld, HitResult.ImpactPoint, ShotDirection.Rotation());
			hitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
		//DrawDebugPoint(GetWorld(), viewPointLocation + viewPointRotation.Vector() * (isAI ? 1 : shooter->GetCameraDistance()), 20, FColor::Red, true);
	}
}