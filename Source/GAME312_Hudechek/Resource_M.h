// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Resource_M.generated.h"

UCLASS()
class GAME312_HUDECHEK_API AResource_M : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource_M();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Set up resource name defaulted to Wood
	UPROPERTY(EditAnywhere)
		FString ResourceName = "Wood";

	//Set up of ResourceAmount to indicated how many resources of this type to receive when gathered
	//Defaulted to 1
	UPROPERTY(EditAnywhere)
		int ResourceAmount = 1;

	//set up TotalResource to tell us the max amount of resource available to gather
	UPROPERTY(EditAnywhere)
		int TotalResource = 100;

	//Used to set our text render to text
	UPROPERTY(EditAnywhere)
		FText TempText;

	//set up pointer to UTextRenderComponant to be able to Render our text
	UPROPERTY(EditAnywhere)
		UTextRenderComponent* ResourceNameText;

	//set up pointer to UStaticMeshComponent so we cal set the Static Mesh in the Editor
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;
	

	
};
