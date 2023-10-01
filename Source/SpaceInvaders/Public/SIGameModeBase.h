// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "SIPawn.h"
#include "SIPlayerController.h"
#include "InvaderSquad.h"

#include "SIGameModeBase.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Layout")
	TSubclassOf<class AInvaderSquad> InvaderSquadClass;

	// Position where squads will be created
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Level Layout")
	FVector spawnLocation;

	UFUNCTION(BlueprintCallable)
	void RegenerateSquad();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	AInvaderSquad* spawnedInvaderSquad;

};
