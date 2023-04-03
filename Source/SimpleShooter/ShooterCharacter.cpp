// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Gun.h"
#include "SimpleShooterGameModeBase.h"

AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	ShooterPlayerController = Cast<APlayerController>(GetController());

	if(ShooterPlayerController)
	{
		if(UEnhancedInputLocalPlayerSubsystem* subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(ShooterPlayerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(TpsShooterInputMappingContext, 0);
		}

	}

	for (TSubclassOf<AGun> GunClass : GunClasses) 
	{
		Guns.Add(GetWorld()->SpawnActor<AGun>(GunClass));
	}

	GetMesh()->HideBoneByName(FName("weapon_r"), EPhysBodyOp::PBO_None);

	for (AGun* Gun : Guns) 
	{
		Gun->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::KeepRelativeTransform,
			FName("WeaponSocket"));

		Gun->SetOwner(this);

		Gun->SetActorHiddenInGame(true);
	}

	Guns[ActiveWeaponIndex]->SetActorHiddenInGame(false);

	/*Guns[ActiveWeaponIndex]->AttachToComponent(
		GetMesh(), 
		FAttachmentTransformRules::KeepRelativeTransform, 
		FName("WeaponSocket"));

	Guns[ActiveWeaponIndex]->SetOwner(this);*/

	camera = FindComponentByClass<UCameraComponent>();
	
}

void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Move);
		enhancedInputComponent->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Look);
		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AShooterCharacter::Jump);
		enhancedInputComponent->BindAction(LookControllerAction, ETriggerEvent::Triggered, this, &AShooterCharacter::LookController);
		enhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AShooterCharacter::Shoot);
		enhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &AShooterCharacter::SwitchWeapon);
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	damageToApply = FMath::Min(Health, damageToApply);
	Health -= damageToApply;

	if(IsDead())
	{
		ASimpleShooterGameModeBase* gameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (gameMode)
		{
			gameMode->PawnKilled(this);
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->DestroyComponent();
	}
	return damageToApply;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0.f;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float AShooterCharacter::GetCameraDistance() const
{
	if (camera) 
	{
		return FVector::Dist(camera->GetComponentLocation(), GetActorLocation());
	}
	return 0.f;
}

void AShooterCharacter::Move(const FInputActionValue& value)
{
	const FVector moveDirection = value.Get<FVector>();
	AddMovementInput(GetActorForwardVector() * moveDirection.Y);
	AddMovementInput(GetActorRightVector() * moveDirection.X);
}

void AShooterCharacter::Look(const FInputActionValue& value)
{
	const FVector lookInput = value.Get<FVector>();
	AddControllerPitchInput(lookInput.Y);
	AddControllerYawInput(lookInput.X);
}

void AShooterCharacter::LookController(const FInputActionValue& value)
{
	const FVector lookInput = value.Get<FVector>();
	AddControllerPitchInput(lookInput.Y * RotationRate * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(lookInput.X * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Jump(const FInputActionValue& value)
{
	ACharacter::Jump();
}

void AShooterCharacter::SwitchWeapon(const FInputActionValue& value)
{
	const float SwitchInput = value.Get<float>();

	Guns[ActiveWeaponIndex]->SetActorHiddenInGame(true);

	if (ActiveWeaponIndex + SwitchInput >= Guns.Num()) 
	{
		ActiveWeaponIndex = 0;
	}
	else if(ActiveWeaponIndex + SwitchInput <  0)
	{
		ActiveWeaponIndex = Guns.Num() - 1;
	}
	else 
	{
		ActiveWeaponIndex += SwitchInput;
	}

	Guns[ActiveWeaponIndex]->SetActorHiddenInGame(false);

}

void AShooterCharacter::Shoot()
{
	Guns[ActiveWeaponIndex]->PullTrigger();
}
