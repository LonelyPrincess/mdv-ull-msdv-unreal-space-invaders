// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Invader.h"

#include "InvaderSquad.generated.h"

UCLASS()
class SPACEINVADERS_API AInvaderSquad : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY()
	class USceneComponent* Root;

	AInvaderSquad();

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	// Values for initializing defaults
	static const int32 defaultNRows = 1;
	static const int32 defaultNCols = 1;
	static constexpr const float defaultHorizontalVelocity = 1000.0f;
	static constexpr const float defaultVerticalVelocity = 1000.0f;
	static constexpr const float defaultExtraSeparation = 0.0f;

};