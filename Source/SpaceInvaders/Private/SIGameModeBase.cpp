// Fill out your copyright notice in the Description page of Project Settings.


#include "SIGameModeBase.h"

// Constructor for GameMode class, which overrides default classes for pawn and player controller
ASIGameModeBase::ASIGameModeBase()
	: spawnLocation{ }
	, spawnedInvaderSquad{}

{

	DefaultPawnClass = ASIPawn::StaticClass();
	PlayerControllerClass = ASIPlayerController::StaticClass();
	InvaderSquadClass = AInvaderSquad::StaticClass();

}


// Method that will run when the object is added to the scene
void ASIGameModeBase::BeginPlay() {

	Super::BeginPlay();

	// Spawn a squad of invaders
	RegenerateSquad();

}

// Method that spawns a new squad on screen
void ASIGameModeBase::RegenerateSquad() {

	// If a previous squad existed, destroy it before generating a new one
	if (this->spawnedInvaderSquad != nullptr) {
		this->spawnedInvaderSquad->Destroy();
	}

	// Generate squad in the specified location
	if (InvaderSquadClass) {
		this->spawnedInvaderSquad = Cast<AInvaderSquad>(GetWorld()->SpawnActor(InvaderSquadClass, &spawnLocation));
	}
}