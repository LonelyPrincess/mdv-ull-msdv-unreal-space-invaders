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

	// Subscribe to event and select function to be called on event trigger
	this->NewSquad.AddUObject(this, &ASIGameModeBase::OnNewSquad);
	this->PlayerZeroLifes.BindUObject(this, &ASIGameModeBase::OnPlayerZeroLifes);

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
		AInvaderSquad* squad = Cast<AInvaderSquad>(InvaderSquadClass->GetDefaultObject());
		this->spawnedInvaderSquad = Cast<AInvaderSquad>(GetWorld()->SpawnActor(InvaderSquadClass, &spawnLocation));
	}
}

// Handler for new squad event
void ASIGameModeBase::OnNewSquad(int32 lifes) {
	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Generating new squad...")));
	RegenerateSquad();
}

// Method that will show message when game is over
void ASIGameModeBase::EndGame() {
	// Destroy squad still alive before ending the game
	if (this->spawnedInvaderSquad != nullptr) {
		this->spawnedInvaderSquad->Destroy();
	}

	// Close game level and show main menu instead
	// UGameplayStatics::OpenLeve(this, FName("Menu"));
	UE_LOG(LogTemp, Display, TEXT("Should exit game now!"));
}

// Handler for event of user losing all their lives
void ASIGameModeBase::OnPlayerZeroLifes() {
	EndGame();
}