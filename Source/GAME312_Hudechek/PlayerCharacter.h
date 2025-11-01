// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class GAME312_HUDECHEK_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//function for forward and back movement defined in CPP
	UFUNCTION()
		void MoveForward(float axisValue);

	//function for forward and back movement defined in CPP
	UFUNCTION()
		void MoveRight(float axisValue);

	//function for forward and back movement defined in CPP
	UFUNCTION()
		void StartJump();

	//function for forward and back movement defined in CPP
	UFUNCTION()
		void StopJump();

	//function for forward and back movement defined in CPP
	UFUNCTION()
		void FindObject();

	// establish pointer to UCameraComponant call PlayerCamera
	UPROPERTY(EditAnywhere)
		UCameraComponent* PlayerCamera;
};
