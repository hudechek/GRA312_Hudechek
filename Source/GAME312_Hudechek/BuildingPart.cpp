// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPart.h"

// Sets default values
ABuildingPart::ABuildingPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create sub object of UStaticMeshComponent 
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//Create sub object of UArrowComponent
	PivotArrow = CreateDefaultSubobject<UArrowComponent>("Pivot Arrow");

	//set RootComponent to PivotArrow 
	RootComponent = PivotArrow;
	//AttachMesh to RootComponent PivotArrow
	Mesh->SetupAttachment(PivotArrow);
}

// Called when the game starts or when spawned
void ABuildingPart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

