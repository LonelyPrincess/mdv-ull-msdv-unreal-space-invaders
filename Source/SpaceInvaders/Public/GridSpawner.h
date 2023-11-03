

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSpawner.generated.h"


UCLASS(Abstract)
class SPACEINVADERS_API AGridSpawner : public AActor
{
	GENERATED_BODY()

public:	

	// Number of rows in the grid
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn settings")
	int32 nRows;

	// Number of columns in the grid
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn settings")
	int32 nCols;

	// Gap between items in the grid
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawn settings")
	float extraSeparation;

	// Rotation to apply to spawned items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn settings")
	FRotator spawnRotation; // = FRotator(0.0f, 180.0f, 0.0f);

	// Sets default values for this actor's properties
	AGridSpawner();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnItems();

	// Methods that need to be implemented in children classes
	// virtual AActor* FetchItemTemplate(); // PURE_VIRTUAL(ITestInterface::Test, ;);
	virtual void OnItemSpawned(AActor* spawnedItem, int32 index) PURE_VIRTUAL(ITestInterface::Test, ;);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
