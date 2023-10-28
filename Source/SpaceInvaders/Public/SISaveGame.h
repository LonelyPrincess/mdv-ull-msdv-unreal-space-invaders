#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SISaveGame.generated.h"

UCLASS()
class SPACEINVADERS_API USISaveGame : public USaveGame
{
	GENERATED_BODY()
	
private:

	// Configure the save slot where data will be stored
	UPROPERTY(VisibleAnywhere, Category = "Save settings")
	FString SaveSlotName;

	// Configure the user to which data will be assigned
	UPROPERTY(VisibleAnywhere, Category = "Save settings")
	uint32 UserIndex;

public:

	// Variable to store the highest score achieved by the player
	UPROPERTY(VisibleAnywhere, Category = "Game statistics")
	int64 highestScore;

	// Class constructor
	USISaveGame();

};
