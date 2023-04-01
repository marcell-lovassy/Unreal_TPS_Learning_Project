// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Gun.h"

AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	ShooterPlayerController = Cast<APlayerController>(GetController());

	if(ShooterPlayerController)
	{
		if(UEnhancedInputLocalPlayerSubsystem* subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(ShooterPlayerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(TpsShooterInputMappingContext, 0);
		}

	}

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(FName("weapon_r"), EPhysBodyOp::PBO_None);

	Gun->AttachToComponent(
		GetMesh(), 
		FAttachmentTransformRules::KeepRelativeTransform, 
		FName("WeaponSocket"));

	Gun->SetOwner(this);
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
	}
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

void AShooterCharacter::Shoot(const FInputActionValue& value)
{
	Gun->PullTrigger();
}
