#include "Barrier.h"
#include "BarrierSegment.h"

// Sets default values
ABarrier::ABarrier()
	: numRows{ 2 }
	, numColumns{ 5 }
	, numActiveSegments{ 0 }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();

	// Subscribe to segment destroyed event
	UWorld* TheWorld = GetWorld();
	SegmentDestroyed.BindUObject(this, &ABarrier::OnSegmentDestroyed);

	SpawnBarrierSegments();
}

// Generate segments to compose a barrier
void ABarrier::SpawnBarrierSegments() {
	float radiusX = 0.0f;
	float radiusY = 0.0f;

	ABarrierSegment* spawnedSegment;
	FVector actorLocation = GetActorLocation();
	FVector spawnLocation = actorLocation;
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (segmentClass != nullptr && segmentClass->IsChildOf<ABarrierSegment>()) {
		spawnParameters.Template = NewObject<ABarrierSegment>(this, segmentClass->GetFName(), RF_NoFlags, segmentClass.GetDefaultObject());
	}
	else {
		spawnParameters.Template = NewObject<ABarrierSegment>();
	}

	// Create a new segment on each cell of the barrier grid
	for (int i = 0; i < this->numColumns; i++)
	{
		for (int j = 0; j < this->numRows; j++)
		{
			spawnedSegment = GetWorld()->SpawnActor<ABarrierSegment>(spawnLocation, spawnRotation, spawnParameters);
			spawnedSegment->SetSegmentIndex(numActiveSegments);
			barrierSegments.Add(spawnedSegment);
			spawnedSegment->SetParent(this);
			this->numActiveSegments += 1;

			// Increase offset for next spawn
			float r = spawnedSegment->GetBoundRadius();
			if (r > radiusX)
				radiusX = r;
			if (r > radiusY)
				radiusY = r;
			spawnLocation.X += radiusX;
		}
		spawnLocation.X = actorLocation.X;
		spawnLocation.Y += radiusY;
	}
	
}

// Called every frame
void ABarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Handler for segment destroyed event
void ABarrier::OnSegmentDestroyed(int32 index) {

	barrierSegments[index] = nullptr;
	this->numActiveSegments -= 1;
	UE_LOG(LogTemp, Warning, TEXT("Barrier %s has %i segments left"), *FString(GetName()), this->numActiveSegments);

	// Destroy barrier once there are no more segments still intact
	if (this->numActiveSegments == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Barrier is down!"));
		Destroy();
	}

}