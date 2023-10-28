#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SIGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API USIGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:

	// Configure the name of the save slot where data will be stored
	FString SaveSlot = TEXT("SISaveSlot");

	// Configure the user to which data will be assigned
	uint32 UserIndex = 0;

	class USISaveGame* SavedGameData;

public: 

	USIGameInstance();

	int64 GetCurrentHighestScore();
	void SaveHighestScore(int64 highestPlayerScore);

};
