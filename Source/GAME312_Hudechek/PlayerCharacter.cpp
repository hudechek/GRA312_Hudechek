// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
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
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//bind inputs Axis controls to player movements to control player and camera
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRightLeft", this, &APlayerCharacter::AddControllerYawInput);

	//Bind input Action controls to KeyBind to control actions and interaction
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);
	//PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);

}

void APlayerCharacter::MoveForward(float axisValue)
{
	//set up FVector variable Direction to hold scaled value of FRotationMatrix X value
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	//AddMovement input to character based on Direction and axisValue
	AddMovementInput(Direction, axisValue);
}

void APlayerCharacter::MoveRight(float axisValue)
{
	//set up FVector variable Direction to hold scaled value of FRotationMatrix Y value
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	//AddMovement input to character based on Direction and axisValue
	AddMovementInput(Direction, axisValue);
}

void APlayerCharacter::StartJump()
{
	//make bPressedJump true when jump action button is pressed
	bPressedJump = true;
}

void APlayerCharacter::StopJump()
{
	//make bPressedJump false when jump action button is Released
	bPressedJump = false;
}

//Future Implementation area
void APlayerCharacter::FindObject()
{
	
}

