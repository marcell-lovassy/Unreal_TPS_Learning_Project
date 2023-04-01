// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"

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
	UE_LOG(LogTemp, Warning, TEXT("%s is shooting"), *GetName());
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, FName("MuzzleFlashSocket"));
	
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (ownerPawn == nullptr) return;

	AController* ownerController = ownerPawn->GetController();
	if (ownerController == nullptr) return;

	FVector viewPointLocation;
	FRotator viewPointRotation;

	ownerController->GetPlayerViewPoint(viewPointLocation, viewPointRotation);
	FVector endPoint = viewPointLocation + viewPointRotation.Vector() * MaxRange;

	//ECC_GameTraceChannel1
	FHitResult hitResult;
	bool isHit = GetWorld()->LineTraceSingleByChannel(hitResult, viewPointLocation, endPoint, ECollisionChannel::ECC_GameTraceChannel1);
	if (isHit)
	{
		FVector shotDirection = -viewPointRotation.Vector();
		//DrawDebugPoint(GetWorld(), hitResult.ImpactPoint, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, hitResult.ImpactPoint, shotDirection.Rotation());
	}

	//DrawDebugCamera(GetWorld(), viewPointLocation, viewPointRotation, 90, 2.f, FColor::Red, true);
}