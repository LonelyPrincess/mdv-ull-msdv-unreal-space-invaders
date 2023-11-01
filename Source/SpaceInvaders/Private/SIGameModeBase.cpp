#include "SIGameModeBase.h"
#include "SIGameInstance.h"

// Constructor for GameMode class, which overrides default classes for pawn and player controller
ASIGameModeBase::ASIGameModeBase()
	: spawnLocation{ }
	, baseSpeedIncrease{ 0.1f }
	, spawnedInvaderSquad{}
	, destroyedSquads{ 0 }
{
	DefaultPawnClass = ASIPawn::StaticClass();
	PlayerControllerClass = ASIPlayerController::StaticClass();
	InvaderSquadClass = AInvaderSquad::StaticClass();
}


// Method that will run when the object is added to the scene
void ASIGameModeBase::BeginPlay() 
{
	Super::BeginPlay();

	// Subscribe to event and select function to be called on event trigger
	this->NewSquad.AddUObject(this, &ASIGameModeBase::OnNewSquad);
	this->PlayerZeroLifes.BindUObject(this, &ASIGameModeBase::OnPlayerZeroLifes);

	// Spawn a squad of invaders
	RegenerateSquad();
}

// Method that spawns a new squad on screen
void ASIGameModeBase::RegenerateSquad() 
{
	// If a previous squad existed, destroy it before generating a new one
	if (this->spawnedInvaderSquad != nullptr) {
		this->spawnedInvaderSquad->Destroy();
	}

	// Generate squad in the specified location
	if (InvaderSquadClass) {
		AInvaderSquad* squad = Cast<AInvaderSquad>(InvaderSquadClass->GetDefaultObject());
		this->spawnedInvaderSquad = Cast<AInvaderSquad>(GetWorld()->SpawnActor(InvaderSquadClass, &spawnLocation));

		// Increase speed of the new squad based on the amount of previously destroyed squads
		if (destroyedSquads > 0) {
			float multiplier = 1.0f + ((float) destroyedSquads * baseSpeedIncrease);
			UE_LOG(LogTemp, Display, TEXT("Speed for new squad will be x%f times faster than the default velocity"), multiplier);
			this->spawnedInvaderSquad->IncreaseSpeed(multiplier);
		}
	}
}

// Handler for new squad event
void ASIGameModeBase::OnNewSquad(int32 lifes) 
{
	destroyedSquads++;
	UE_LOG(LogTemp, Display, TEXT("%i squads have been destroyed"), destroyedSquads);

	UE_LOG(LogTemp, Display, TEXT("Generating new squad..."));
	RegenerateSquad();
}

// Method that will show message when game is over
void ASIGameModeBase::EndGame() 
{
	// Destroy squad still alive before ending the game
	if (this->spawnedInvaderSquad != nullptr) {
		this->spawnedInvaderSquad->Destroy();
	}

	// Close game level and show main menu instead
	UE_LOG(LogTemp, Display, TEXT("Should exit game now!"));
	UGameplayStatics::OpenLevel(this, FName("Menu"));
}

// Handler for event of user losing all their lives
void ASIGameModeBase::OnPlayerZeroLifes(int64 playerScore) 
{
	// Update highest score if current score exceeds previous record
	USIGameInstance* gameInstance = Cast<USIGameInstance>(this->GetGameInstance());
	int64 currentHighestScore = gameInstance->GetCurrentHighestScore();
	if (playerScore > currentHighestScore) {
		gameInstance->UpdateHighestScore(playerScore);
	}

	EndGame();
}