// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
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

	//function for right and left movement defined in CPP
	UFUNCTION()
		void MoveRight(float axisValue);

	//function for jumping movement defined in CPP
	UFUNCTION()
		void StartJump();

	//function for canceling jump logic defined in CPP
	UFUNCTION()
		void StopJump();

	//function to find object that is being looked at defined in CPP
	UFUNCTION()
		void FindObject();

	// establish pointer to UCameraComponant call PlayerCamera
	UPROPERTY(EditAnywhere)
		UCameraComponent* PlayerCamera;
	
	// set default Value of Health to 100.0f using float for decimal values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Health = 100.0f;
	
	// set default Value of Hunger to 100.0f using float for decimal values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Hunger = 100.0f;
	
	// set default Value of Stamina to 100.0f using float for decimal values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Stamina = 100.0f;

	//int value for wood 
	UPROPERTY(EditAnywhere, Category = "Player Inventory")
		int Wood;

	//int value for stone
	UPROPERTY(EditAnywhere, Category = "Player Inventory")
		int Stone;

	//int value for Berries
	UPROPERTY(EditAnywhere, Category = "Player Inventory")
		int Berries;

	//Array used to hold all the Resource amounts 
	UPROPERTY(EditAnywhere, Category = "Player Inventory")
		TArray<int> ResourceArray;

	//Array to hold all the names of Resources
	UPROPERTY(EditAnywhere, Category = "Player Inventory")
		TArray<FString> ResourceNameArray;

	UPROPERTY(EditAnywhere, Category = "Hit Marker")
		UMaterialInterface* HitDecal;
	
	//function to set Health variable to new amount
	UFUNCTION(BlueprintCallable)
		void SetHealth(float amount);

	//function to set Hunger variable to new amount
	UFUNCTION(BlueprintCallable)
		void SetHunger(float amount);

	//function to set Stamina variable to new amount
	UFUNCTION(BlueprintCallable)
		void SetStamina(float amount);

	//used to decrease states for defined amounts
	UFUNCTION()
		void DecreaseStats();
	
	//function for giving player resource within array defined in cpp
	//parameters for amount and resource name
	UFUNCTION()
		void GiveResource(float amount, FString resourceType);
};
