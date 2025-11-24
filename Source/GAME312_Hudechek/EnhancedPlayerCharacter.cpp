// Fill out your copyright notice in the Description page of Project Settings.

#include "EnhancedPlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"


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

	//add input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		//Get local player subsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (Movement)
		{
			EnhancedInput->BindAction(Movement, ETriggerEvent::Triggered, this, &AEnhancedPlayerCharacter::Move);
		}
		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AEnhancedPlayerCharacter::HandleJump);
		}
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this , &AEnhancedPlayerCharacter::HandleLook);
		}
	}
}


void AEnhancedPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2d MovementVector = Value.Get<FVector2d>();

	const FRotator CameraYawRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, CameraYawRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AEnhancedPlayerCharacter::HandleJump(const FInputActionValue& Value)
{
	Jump();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Jump");
}

void AEnhancedPlayerCharacter::HandleLook(const FInputActionValue& Value)
{
	const FVector2d LookDirection = Value.Get<FVector2d>();
	AddControllerPitchInput(LookDirection.Y * UGameplayStatics::GetWorldDeltaSeconds(this));
	AddControllerYawInput(LookDirection.X * UGameplayStatics::GetWorldDeltaSeconds(this));
	
}