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

	// Class that will be used to instantiate the squad members
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Squad Spawner")
	TSubclassOf<class AInvader> invaderClass;

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

	UPROPERTY()
	class AInvader* invaderTemplate;

	// Variable where we'll store how much time has passed since an invader entered free jump mode
	float timeFromLastFreeJump;

	// Values for initializing defaults
	static const int32 defaultNRows = 1;
	static const int32 defaultNCols = 1;
	static constexpr const float defaultHorizontalVelocity = 1000.0f;
	static constexpr const float defaultVerticalVelocity = 1000.0f;
	static constexpr const float defaultExtraSeparation = 0.0f;

};