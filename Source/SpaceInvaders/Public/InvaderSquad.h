// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Invader.h"
#include "InvaderMovementComponent.h"

#include "InvaderSquad.generated.h"

// Forward declaration of this enum, when using "class" here we're telling the compiler this is defined elsewhere (the invader class, in this case)
enum class InvaderMovementType : uint8;

// Struct to define an invader class and how likely it'd be for it to spawn
USTRUCT(BlueprintType)
struct FInvaderClassStruct {
	GENERATED_BODY()

	// Class that will be used to spawn invaders in the squad
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Invader type")
	TSubclassOf<class AInvader> invaderClass;

	// The larger this number, the larger the odds for this particular class to be spawned when creating a squad
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Invader type")
	int32 spawnOdds;
};

UCLASS()
class SPACEINVADERS_API AInvaderSquad : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY()
	class USceneComponent* Root;

	// Variable with current movement type of the whole squad
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Squad movement")
	InvaderMovementType state;

	// Previous movement type requested by squad controller
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Squad movement")
	InvaderMovementType previousState;

	// Determine how frequent it's that a invader enters free jump mode
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Squad movement")
	float freeJumpRate;

	// Overall speed of the squad
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Squad movement")
	float horizontalVelocity;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Squad movement")
	float verticalVelocity;

	AInvaderSquad();

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function that's used to set the movement type that will be applied to the whole squad
	UFUNCTION(BlueprintCallable)
	void UpdateSquadState(float delta);

protected:

	// Classes that will be used to instantiate the squad members
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Squad Spawner")
	TArray<FInvaderClassStruct> invaderClasses;

	// Number of rows in squad
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Squad Spawner")
	int32 nRows;

	// Number of invaders on each column
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Squad Spawner")
	int32 nCols;

	// Gap between squad members
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Squad Spawner")
	float extraSeparation;

	// Array where all squad members are stored
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<class AInvader*> SquadMembers;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Method that's called when squad is destroyed
	virtual void Destroyed() override;

private:

	int32 numberOfMembers;

	TArray<int32> oddsArray;
	TArray<class AInvader*> invaderTemplates;

	void InitializeInvaderTemplates();
	class AInvader* FetchRandomInvaderTemplate();

	// Variable where we'll store how much time has passed since an invader entered free jump mode
	float timeFromLastFreeJump;

	// Values for initializing defaults
	static const int32 defaultNRows = 1;
	static const int32 defaultNCols = 1;
	static constexpr const float defaultHorizontalVelocity = 1000.0f;
	static constexpr const float defaultVerticalVelocity = 1000.0f;
	static constexpr const float defaultExtraSeparation = 0.0f;

private:

	// Event handlers
	void SquadOnLeftSide();
	void SquadOnRightSide();
	void SquadFinishesDown();
	void RemoveInvader(int32 ind);

	// Reference to game mode, which will be used to listen or emit events
	UPROPERTY()
	class ASIGameModeBase* MyGameMode;

public:

	// Increases movement speed for the squad
	void IncreaseSpeed(float multiplier);

};