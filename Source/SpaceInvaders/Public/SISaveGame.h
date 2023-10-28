#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SISaveGame.generated.h"

UCLASS()
class SPACEINVADERS_API USISaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	// Variable to store the highest score achieved by the player
	UPROPERTY(VisibleAnywhere, Category = "Game statistics")
	int64 HighestScore;

};
