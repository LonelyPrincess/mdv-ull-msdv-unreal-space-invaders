// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderSquad.h"

// Sets default values
// Order in which params show up in initialization list must match the order in which props were defined in the defintion file (".h")
AInvaderSquad::AInvaderSquad()
	: state{ InvaderMovementType::STOP }
	, previousState{ InvaderMovementType::STOP }
	, freeJumpRate{ 0.0001 }
	, horizontalVelocity{ 300.0 }
	, verticalVelocity{ 300.0 }
	, nRows{ AInvaderSquad::defaultNRows }
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

	// By default, move to right
	this->state = InvaderMovementType::RIGHT;

}

// Called every frame
void AInvaderSquad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSquadState(DeltaTime);
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

// Update movement direction of the squad based on current conditions
void AInvaderSquad::UpdateSquadState(float delta) {

	// Here we'll store all invaders that are still alive
	TArray<AInvader*> survivors;

	for (auto invader : SquadMembers) {
		if (invader) { // If invader is not null, it means it's still part of the squad

			// First, we get the movement component
			UInvaderMovementComponent* imc = (UInvaderMovementComponent*)invader->GetComponentByClass(UInvaderMovementComponent::StaticClass());

			// Now, its state is updated
			if (imc) {
				// We add invader to survivor list only if they're not currently in free jump mode
				if (imc->state != InvaderMovementType::FREEJUMP) {
					survivors.Emplace(invader);
					imc->horizontalVelocity = horizontalVelocity;
					imc->verticalVelocity = verticalVelocity;
					imc->state = state;
				}
			}
		}
	}

	// We fetch random number that will determine if any of the survivors also enter free jump mode
	this->timeFromLastFreeJump += delta;
	float val = FMath::RandRange(0.0f, 1.0f);
	int32 countSurvivors = survivors.Num();

	if (countSurvivors > 0 && val < (1.0 - FMath::Exp(-freeJumpRate * this->timeFromLastFreeJump))) {
		UE_LOG(LogTemp, Display, TEXT("One of the squad units will enter free jump mode!"));
		int32 ind = FMath::RandRange(0, countSurvivors - 1); // Randomly select one of the living invaders
		UInvaderMovementComponent* imc = (UInvaderMovementComponent*)survivors[ind]->GetComponentByClass(UInvaderMovementComponent::StaticClass());
		if (imc) {
			// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%s on FreeJump"), *(imc->GetName())));
			// Fire rate for bersek invaders will increase drastically, so they're more likely to attack the player
			survivors[ind]->fireRate *= 100;
			imc->state = InvaderMovementType::FREEJUMP;
		}
	}
}