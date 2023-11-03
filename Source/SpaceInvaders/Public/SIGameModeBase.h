#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "SIPawn.h"
#include "SISaveGame.h"
#include "SIPlayerController.h"
#include "InvaderSquad.h"

#include "SIGameModeBase.generated.h"

// Delegates of this game:
DECLARE_DELEGATE(FStandardDelegateSignature)
DECLARE_MULTICAST_DELEGATE_OneParam(FOneParamMulticastDelegateSignature, int32)
DECLARE_DELEGATE_OneParam(FOneParamDelegateSignature, int32)
DECLARE_DELEGATE_OneParam(FOneLongIntParamDelegateSignature, int64)
DECLARE_MULTICAST_DELEGATE_TwoParams(FTwoParamsMulticastDelegateSignature, int32, bool)
DECLARE_DELEGATE_OneParam(FSegmentDestroyedDelegateSignature, int32);

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ASIGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ASIGameModeBase();

public:

	// Class that will be used to spawn squads
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad settings")
	TSubclassOf<class AInvaderSquad> InvaderSquadClass;

	// Position where squads will be created
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Squad settings")
	FVector spawnLocation;

	// Base speed increase that'll be used to make new squads move faster
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Squad settings")
	float baseSpeedIncrease;

	UFUNCTION(BlueprintCallable)
	void RegenerateSquad();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	AInvaderSquad* spawnedInvaderSquad;

	int32 destroyedSquads;

public:

	// We create delegates for supported event types		// Emitter -> Subscriber(s)
	FStandardDelegateSignature SquadOnLeftSide;				// Invader -> Squad 
	FStandardDelegateSignature SquadOnRightSide;			// Invader -> Squad
	FStandardDelegateSignature SquadFinishesDown;			// Invader -> Squad
	FStandardDelegateSignature SquadSuccessful;				// Invader -> Game Mode
	FTwoParamsMulticastDelegateSignature InvaderDestroyed;	// Invader -> Squad, Player

	FOneParamMulticastDelegateSignature NewSquad;			// Squad -> Game Mode 
	FOneLongIntParamDelegateSignature PlayerZeroLifes;		// Player -> Game Mode

	FSegmentDestroyedDelegateSignature SegmentDestroyed;	// Barrier segment -> Barrier

protected:

	// Delegate bindings (event handling functions)
	UFUNCTION(BlueprintCallable)
	void OnNewSquad(int32 lifes);

	void EndGame();

	UFUNCTION(BlueprintCallable)
	void OnPlayerZeroLifes(int64 playerScore);

};
