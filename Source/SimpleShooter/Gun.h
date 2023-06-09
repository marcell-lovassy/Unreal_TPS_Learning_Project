// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "GunProperties")
	float Damage = 10.f;
	UPROPERTY(EditDefaultsOnly, Category = "GunProperties")
	float MaxRange = 10000;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ImpactEffectWorld; 
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ImpactEffectCharacter;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* MuzzleSound;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* ImpactSound;

	bool GunTrace(FHitResult& HitResult, FVector& ShotDirection);

	AController* GetOwnerController(class AShooterCharacter*& shooter) const;

};
