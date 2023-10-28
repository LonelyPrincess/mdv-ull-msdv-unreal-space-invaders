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
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Previous save data loaded"));
	}
}

/*USISaveGame* USIGameInstance::LoadGameData()
{
	if (SavedGameData) {
		UE_LOG(LogTemp, Warning, TEXT("Stored score: %i"), SavedGameData->HighestScore);
		return SavedGameData;
	}
	else
		return nullptr;
}*/

int64 USIGameInstance::GetCurrentHighestScore() 
{
	if (SavedGameData) {
		return SavedGameData->HighestScore;
	}

	return 0;
}

void USIGameInstance::SaveHighestScore(int64 highestPlayerScore)
{
	/*if (SavedGameData) {
		SavedGameData->HighestScore = highestPlayerScore;
		if (UGameplayStatics::SaveGameToSlot(SavedGameData, SaveSlot, 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("New highest score %i was saved"), highestPlayerScore);
		}
	}*/

	if (SavedGameData)
	{
		// Set up the (optional) delegate.
		FAsyncSaveGameToSlotDelegate SavedDelegate;
		// USomeUObjectClass::SaveGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, bool bSuccess
		// SavedDelegate.BindUObject(SomeUObjectPointer, &USomeUObjectClass::SaveGameDelegateFunction);

		// Set data on the savegame object.
		SavedGameData->HighestScore = highestPlayerScore;

		// Start async save process.
		UGameplayStatics::AsyncSaveGameToSlot(SavedGameData, SaveSlot, UserIndex, SavedDelegate);
	}

	/*if (SavedGameData)
	{
		SavedGameData->HighestScore = highestPlayerScore;
		UGameplayStatics::SaveGameToSlot(SavedGameData, SaveSlot, 0);
		if (isGameSaved) {
			UE_LOG(LogTemp, Warning, TEXT("New highest score was successfully saved!"));
		}


		SavedGameData->HighestScore = 0;

		SaveGameInstance->highestScore = highestPlayerScore;
		if (UGameplayStatics::SaveGameToSlot(SavedGameData, SaveSlot, 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Game saved!"));
		}

		SaveGameInstance->highestScore = playerScore;
		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("Game saved!"));
		}
	}*/
}