#include "SIGameInstance.h"

#include "SISaveGame.h"
#include "Kismet/GameplayStatics.h"

USIGameInstance::USIGameInstance()
{
	UE_LOG(LogTemp, Display, TEXT("Game instance created"));

	// Attempt to load previous save data as soon as the game starts
	SavedGameData = Cast<USISaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, UserIndex));

	// If no previous data is found, create a new save slot
	if (!SavedGameData)
	{
		UE_LOG(LogTemp, Warning, TEXT("No previous game data found, so we create a new slot..."));
		SavedGameData = Cast<USISaveGame>(UGameplayStatics::CreateSaveGameObject(USISaveGame::StaticClass()));

		SavedGameData->HighestScore = 0;

		UGameplayStatics::SaveGameToSlot(SavedGameData, SaveSlot, UserIndex);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Previous save data loaded"));
	}
}

int64 USIGameInstance::GetCurrentHighestScore() 
{
	if (SavedGameData) {
		return SavedGameData->HighestScore;
	}

	return 0;
}

void USIGameInstance::UpdateHighestScore(int64 highestPlayerScore)
{
	if (SavedGameData) {
		SavedGameData->HighestScore = highestPlayerScore;
		if (UGameplayStatics::SaveGameToSlot(SavedGameData, SaveSlot, UserIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("Highest score is now %i"), highestPlayerScore);
		}
	}
}