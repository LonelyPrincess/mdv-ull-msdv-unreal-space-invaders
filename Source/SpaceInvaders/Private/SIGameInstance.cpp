#include "SIGameInstance.h"

#include "SISaveGame.h"
#include "Kismet/GameplayStatics.h"

// Attempt to load previous save data as soon as the game starts
USIGameInstance::USIGameInstance()
{
	UE_LOG(LogTemp, Display, TEXT("Game instance created"));

	SavedGameData = Cast<USISaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));

	// If no previous data is found, create a new save slot
	if (!SavedGameData)
	{
		UE_LOG(LogTemp, Warning, TEXT("No previous game data found, so we create a new slot..."));
		SavedGameData = Cast<USISaveGame>(UGameplayStatics::CreateSaveGameObject(USISaveGame::StaticClass()));

		SavedGameData->HighestScore = 0;

		UGameplayStatics::SaveGameToSlot(SavedGameData, SaveSlot, 0);
	}
}

USISaveGame* USIGameInstance::LoadGameData()
{
	if (SavedGameData) {
		UE_LOG(LogTemp, Warning, TEXT("Stored score: %i"), *SavedGameData->HighestScore);
		return SavedGameData;
	}
	else
		return nullptr;
}

void USIGameInstance::SaveHighestScore(int64 highestPlayerScore) 
{
	if (SavedGameData)
	{
		SavedGameData->HighestScore = highestPlayerScore;
		bool isGameSaved = UGameplayStatics::SaveGameToSlot(SavedGameData, SaveSlot, 0);
		if (isGameSaved) {
			UE_LOG(LogTemp, Warning, TEXT("New highest score was successfully saved!"));
		}
	}
}