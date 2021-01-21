// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformingCharacter.h"

// Sets default values
APlatformingCharacter::APlatformingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlatformingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlatformingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlatformingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Turn", this, &APlatformingCharacter::Turn);
	PlayerInputComponent->BindAxis("Look", this, &APlatformingCharacter::Look);
	PlayerInputComponent->BindAxis("Forward", this, &APlatformingCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &APlatformingCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlatformingCharacter::Jump_Pressed);
}

void APlatformingCharacter::Look(float Value)
{
	AddControllerPitchInput(Value);
}

void APlatformingCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void APlatformingCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void APlatformingCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void APlatformingCharacter::Jump_Pressed()
{
	Jump();
}
