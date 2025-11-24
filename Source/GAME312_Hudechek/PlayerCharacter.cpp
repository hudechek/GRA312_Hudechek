// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"

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

	//set up buildingArray to 3 for each resource
	BuildingArray.SetNum(3);
	
	//set up of rudimentary inventory system;
	ResourceArray.SetNum(3);
	ResourceNameArray.Add(TEXT("Wood"));
	ResourceNameArray.Add(TEXT("Stone"));
	ResourceNameArray.Add(TEXT("Berries"));

	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//constructor for StatsTimerHandle
	FTimerHandle StatsTimerHandle;
	//Set timer using StatsTimerHandle to call DecreaseStats ever 2 Seconds on a loop
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerCharacter::DecreaseStats, 2, true);
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//call UpdateHudBars function every from class HUD pass in Health Hunger and Stamina Variables
	HUD->UpdateHudBars(Health, Hunger, Stamina);

	//determine if we are building
	if (isBuilding)
	{
		//determine if spawned part is a vaild object
		if (spawnedBuildingPart)
		{
			//Start Location of trace
			FVector Startlocation = PlayerCamera->GetComponentLocation();
			//Cameras direction in world
			FVector Direction = PlayerCamera->GetForwardVector() * 800.0f;
			//EndLocation of trace for placing object 
			FVector EndLocation = Startlocation + Direction;
			//spawns spawnedBuildingPart at the EndLocation of trace 
			spawnedBuildingPart->SetActorLocation(EndLocation);
		}
		
	}
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
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::FindObject);
	PlayerInputComponent->BindAction("RotatePart", IE_Pressed, this, &APlayerCharacter::rotateBuilding);

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
	// set up HitResult object of struct FHit Result
	FHitResult HitResult;
	//set up start location of Line trace from player camera component location
	FVector StartLocation = PlayerCamera->GetComponentLocation();
	//set up direction of the line trace in the direction the player is looking * 800 units into space
	FVector Direction = PlayerCamera->GetForwardVector() * 800.0f;
	//Determine EndLocation by adding the StartLoaction to Direction
	FVector EndLocation = StartLocation + Direction;

	//set up object to hold Querry Parameters
	FCollisionQueryParams QueryParams;
	//add PlayerCharacter to the list of ignored objects
	QueryParams.AddIgnoredActor(this);
	//Enable Complex Trace Results
	QueryParams.bTraceComplex = true;
	//Enable returning normal of object for when needed
	QueryParams.bReturnFaceIndex = true;

	//Check if we are in building mode or not to determine if we should be able to gather resources
	if (!isBuilding)
	{
		//preform lintrace and do logic if trace comes back with a success
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation,ECC_Visibility, QueryParams))
		{
			//cast to Resorce_M and get a refrence to the actor and store it as HitResult
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());
			if (Stamina > 5.0f)
			{
				if (HitResource)
				{
					//store hit resources name in local variable of hitName
					FString hitName = HitResource->ResourceName;
					//store hit resources resource amount in ResourceValue
					int ResourceValue = HitResource->ResourceAmount;

					//reduce the Total available Resource by the amount taken based on ResourceAmount
					HitResource->TotalResource = HitResource->TotalResource - ResourceValue;

					//if if the hit resources total value is greater than the Current amount being taken
					if (HitResource->TotalResource > ResourceValue)
					{
						//call GiveResource and passing in the local variable of ResourceValue for amount to give player and htiName to determine which index to allocate the amount to
						GiveResource(ResourceValue, hitName);
				
						//check to make sure GEngine is not null
						check (GEngine != nullptr)
				
						//Display Resource Collected Message
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Resource Collected"));

						UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, FVector(10.0f, 10.0f,10.0f), HitResult.Location, FRotator(-90.0f, 0.0f, 0.0f), 2.0f);
					
						//subtract 5 from stamina when gathering is sucessful
						SetStamina(-5.0f);
					}
					//if the hit resource is anything other than greater than destroy the resource
					else
					{
						//destroy hit resorce
						HitResource->Destroy();
				
						//check to make sure GEngine is not null
						check (GEngine != nullptr)
				
						//Display Resource Depleted Message
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Resource Depleted"));
					}

				
				}
		
			}
		}
		
	}
	else
	{
		isBuilding = false;
		
	}
}

//Sets player Health to passed in amount
void APlayerCharacter::SetHealth(float amount)
{
	//if health plus the amount passed in is less than 100.0f set health to health + amount 
	if (Health + amount <= 100.0f)
	{
		Health = Health + amount;
	}
}

//sets player hunger to passed in amount
void APlayerCharacter::SetHunger(float amount)
{
	//if health plus the amount passed in is less than 100.0f set hunger to hunger + amount
	if (Hunger + amount <= 100.0f)
	{
		Hunger = Hunger + amount;
	}
}

//sets player stamina to passed in amount
void APlayerCharacter::SetStamina(float amount)
{
	//if health plus the amount passed in is less than 100.0f set stamina to stamina + amount
	if (Stamina + amount < 100.0f)
	{
		Stamina = Stamina + amount;
	}
}

//called Via Timer to decrease stats based on set interior code
void APlayerCharacter::DecreaseStats()
{
	//used to ensure function is called appropriately by timer
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Decrease Stats Called!"));

	//if Hunger is greater than 100 then call SetHunger and pass in -1 to decrease Hunger
	if (Hunger > 0.0f )
	{
		SetHunger(-1.0f);
	}

	//every2 seconds increase hunger by 10
	SetStamina(10.0f);

	//if Hunger is less than or equal to 0 call SetHealth and pass in -1 to decrease Health
	if (Hunger <= 0.0f)
	{
		SetHealth(-1.0f);
	}
}

//function to give the player resources based on resourceType name and giving amount 
void APlayerCharacter::GiveResource(float amount, FString resourceType)
{
	//check if wood hitName and add amount if true
	if (resourceType == "wood")
	{
		ResourceArray[0] = ResourceArray[0] + amount;	
	}
	//check if Stone hitName and add amount if true
	if (resourceType == "stone")
	{
		ResourceArray[1] = ResourceArray[1] + amount;
	}
	//Check if Berries hitName and add amount if true
	if (resourceType == "Berries")
	{
		ResourceArray[2] = ResourceArray[2] + amount;
	}
}

//function definition to update the number of resources in the resourceArray and update the array of building parts we have
void APlayerCharacter::UpdateResources(float WoodAmount, float StoneAmount, FString buildingObject)
{
	ResourceArray[0] = ResourceArray[0] - WoodAmount;
	ResourceArray[1] = ResourceArray[1] - StoneAmount;

	if (buildingObject == "Wall")
	{
		BuildingArray[0] = BuildingArray[0] + 1;
	}
	if (buildingObject == "Floor")
	{
		BuildingArray[1] = BuildingArray[1] + 1;
	}
	if (buildingObject == "Ceiling")
	{
		BuildingArray[2] = BuildingArray[2] + 1;
	}
}

//take the building id based on array index from building array and spawn actor into world based on trace location
void APlayerCharacter::SpawnBuilding(int BuildingID, bool& isSuccess)
{
	if (!isBuilding)
	{
		if (BuildingArray[BuildingID] >= 1)
		{
			isBuilding = true;
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = PlayerCamera->GetComponentLocation();
			FVector Direction = PlayerCamera->GetForwardVector() * 800.0f;
			FVector EndLocation = StartLocation + Direction;
			FRotator MyRot(0,0,0);

			BuildingArray[BuildingID] = BuildingArray[BuildingID] - 1;

			spawnedBuildingPart = GetWorld()->SpawnActor<ABuildingPart>(BuildingPartClass, EndLocation, MyRot, SpawnParams);

			isSuccess = true;
		}
		else
		{
			isBuilding = false;
		}
		
	}
}

//rotate building 90 degrees in the yaw based on pivot pointset in buildingPartBlueprint
void APlayerCharacter::rotateBuilding()
{
	if (isBuilding)
	{
		spawnedBuildingPart->AddActorWorldRotation(FRotator(0,90,0));
	}
}

