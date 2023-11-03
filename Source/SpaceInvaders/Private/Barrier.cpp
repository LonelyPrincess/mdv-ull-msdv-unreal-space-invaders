


#include "Barrier.h"
#include "BarrierSegment.h"
#include "SIGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABarrier::ABarrier()
	: nRows{ 2 }
	, nCols{ 5 }
	, numSegments{ 0 }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();

	UWorld* TheWorld = GetWorld();

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
	ASIGameModeBase* MyGameMode = Cast<ASIGameModeBase>(GameMode);

	if (MyGameMode != nullptr) {
		MyGameMode->SegmentDestroyed.BindUObject(this, &ABarrier::OnSegmentDestroyed);
	}

	SpawnBarrierSegments();
}

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
	for (int i = 0; i < this->nCols; i++)
	{
		for (int j = 0; j < this->nRows; j++)
		{
			spawnedSegment = GetWorld()->SpawnActor<ABarrierSegment>(spawnLocation, spawnRotation, spawnParameters);
			spawnedSegment->SetSegmentIndex(numSegments);
			barrierSegments.Add(spawnedSegment);
			this->numSegments += 1;

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

// TODO: this event is being received by the two barriers!! 
// We need to identify if the segment belongs to this instance or not, otherwise we don't have to delete anything
// TODO: destroy when no more segments are alive
void ABarrier::OnSegmentDestroyed(int32 index) {

	barrierSegments[index] = nullptr;
	this->numSegments -= 1;
	UE_LOG(LogTemp, Warning, TEXT("Barrier has %i segments left"), this->numSegments);
	if (this->numSegments == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Barrier is down!"));
		// Destroy();
	}

}