// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderSquad.h"

// Sets default values
AInvaderSquad::AInvaderSquad()
	: nRows{ AInvaderSquad::defaultNRows }
	, nCols{ AInvaderSquad::defaultNCols }
	, extraSeparation(AInvaderSquad::defaultExtraSeparation)
	, numberOfMembers{ nRows * nCols }
{
	PrimaryActorTick.bCanEverTick = true;

	// Create scene component, to apply geo transform to the squad (even though it won't be visible)
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root; // We need a RootComponent to have a base transform

}

// Called when the game starts or when spawned
void AInvaderSquad::BeginPlay()
{
	Super::BeginPlay();

	// Set Invader Template with default value for invaderClass
	if (invaderClass != nullptr && invaderClass->IsChildOf<AInvader>())
		invaderTemplate = NewObject<AInvader>(this, invaderClass->GetFName(), RF_NoFlags, invaderClass.GetDefaultObject());
	else {
		invaderClass = AInvader::StaticClass();
		invaderTemplate = NewObject<AInvader>();
	}

	// Spawn Invaders
	FVector actorLocation = GetActorLocation();
	FVector spawnLocation = actorLocation;
	FRotator spawnRotation = FRotator(0.0f, 180.0f, 0.0f); // Invader Forward is oposite to Player Forward (Yaw rotation)
	FActorSpawnParameters spawnParameters;
	int32 count = 0;
	AInvader* spawnedInvader;
	float radiusX = 0.0f;
	float radiusY = 0.0f;

	// Create a new invader on each cell of the squad grid
	for (int i = 0; i < this->nCols; i++)
	{
		for (int j = 0; j < this->nRows; j++)
		{
			spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			spawnParameters.Template = invaderTemplate;
			spawnedInvader = GetWorld()->SpawnActor<AInvader>(spawnLocation, spawnRotation, spawnParameters);
			spawnedInvader->SetPositionInSquad(count);
			++count;
			SquadMembers.Add(spawnedInvader);
			float r = spawnedInvader->GetBoundRadius();
			if (r > radiusX)
				radiusX = r;
			if (r > radiusY)
				radiusY = r;
			spawnLocation.X += radiusX * 2 + this->extraSeparation;
		}
		spawnLocation.X = actorLocation.X;
		spawnLocation.Y += radiusY * 2 + this->extraSeparation;
	}

	this->numberOfMembers = count;

}

// Called every frame
void AInvaderSquad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Method that will run when squad is destroyed
void AInvaderSquad::Destroyed() {

	// Remove all invader objects in the squad from the scenes
	for (AInvader* invader : SquadMembers) {
		if (invader != nullptr)
			invader->Destroy();
	}

	Super::Destroyed();

}