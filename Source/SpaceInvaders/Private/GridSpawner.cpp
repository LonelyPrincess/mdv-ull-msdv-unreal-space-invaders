


#include "GridSpawner.h"

// Sets default values
AGridSpawner::AGridSpawner()
	: nRows{ 1 }
	, nCols{ 1 }
	, extraSeparation(0.0f)
	, spawnRotation{ FRotator(0.0f, 0.0f, 0.0f) }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridSpawner::BeginPlay()
{
	Super::BeginPlay();

}

void AGridSpawner::SpawnItems()
{
	/*FVector actorLocation = GetActorLocation();
	FVector spawnLocation = actorLocation;
	
	int32 count = 0;
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* spawnedActor;
	float radiusX = 0.0f;
	float radiusY = 0.0f;

	// Create a new invader on each cell of the squad grid
	for (int i = 0; i < this->nCols; i++)
	{
		for (int j = 0; j < this->nRows; j++)
		{
			spawnParameters.Template = FetchItemTemplate();
			spawnedActor = GetWorld()->SpawnActor<AActor>(spawnLocation, spawnRotation, spawnParameters);
			OnItemSpawned(spawnedActor, count);
			++count;

			float r = 0.0f; // spawnedActor->GetBoundRadius();
			if (r > radiusX)
				radiusX = r;
			if (r > radiusY)
				radiusY = r;
			spawnLocation.X += radiusX * 2 + this->extraSeparation;
		}

		spawnLocation.X = actorLocation.X;
		spawnLocation.Y += radiusY * 2 + this->extraSeparation;
	}
	*/
}

// Called every frame
void AGridSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

