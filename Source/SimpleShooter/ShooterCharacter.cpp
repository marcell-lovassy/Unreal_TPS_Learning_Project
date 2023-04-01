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
	}
}

void AShooterCharacter::Move(const FInputActionValue& value)
{
	//now the Y is the forward-backward direction and the X is the sideways direction
	const FVector moveDirection = value.Get<FVector>();
	//UE_LOG(LogTemp, Warning, TEXT("Movement: (%f ; %f)"), moveDirection.X, moveDirection.Y);
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
