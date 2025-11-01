// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancedPlayerCharacter.h"

// Sets default values
AEnhancedPlayerCharacter::AEnhancedPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialize playerCamera for playerCharacter
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));

	//Attach Camera to head socket of the UE mannequin
	PlayerCamera->SetupAttachment(GetMesh(), "head");

	//sets up link to pawn controls for 
	PlayerCamera->bUsePawnControlRotation = true;

	

}

// Called when the game starts or when spawned
void AEnhancedPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(GRA312MappingContext, 0);
		}
	}
	
}

// Called every frame
void AEnhancedPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnhancedPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (Movement)
		{
			EnhancedInput->BindAction(Movement, ETriggerEvent::Triggered, this, &AEnhancedPlayerCharacter::Move);
		}
		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AEnhancedPlayerCharacter::Jump);
		}
	}
}

void AEnhancedPlayerCharacter::Jump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump(Value);
	}
}

void AEnhancedPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2d MovementVector = Value.Get<FVector2d>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}
