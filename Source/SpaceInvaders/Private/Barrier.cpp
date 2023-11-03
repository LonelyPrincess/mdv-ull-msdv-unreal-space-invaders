


#include "Barrier.h"
#include "BarrierSegment.h"

// Sets default values
ABarrier::ABarrier()
	: nRows{ 2 }
	, nCols{ 5 }
{
 	/*// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create empty root component to apply transformations
	USceneComponent* ContainerComponent = CreateDefaultSubobject<USceneComponent>("BarrierContainer");
	RootComponent = ContainerComponent;

	numSegments = 3;

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(ABarrier::defaultStaticMeshPath);
	UStaticMesh* staticMesh = MeshAsset.Object;

	for (int i = 0; i < numSegments; i++) {
		UStaticMeshComponent* SegmentMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BarrierSegmentMesh" + (i + 1));
		SegmentMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		SegmentMeshComponent->SetStaticMesh(staticMesh);

		float segmentScale = 1.0f;

		SegmentMeshComponent->SetWorldScale3D(FVector(segmentScale, segmentScale, segmentScale / 2.0f));
		SegmentMeshComponent->SetRelativeScale3D(FVector(segmentScale, segmentScale, segmentScale / 2.0f));

		FVector Position;
		Position.X = i * segmentScale * 100;
		Position.Y = 0.0f;
		Position.Z = 0.0f;

		UE_LOG(LogTemp, Warning, TEXT("Trying to change location for segment %i to X = %i"), i + 1, Position.X);

		// SegmentMeshComponent->SetMobility(EComponentMobility::Movable);
		SegmentMeshComponent->SetMobility(EComponentMobility::Movable);
		SegmentMeshComponent->SetWorldLocation(Position);

		barrierSegments.Add(SegmentMeshComponent);
	}*/
}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();

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
			// spawnedSegment->SetPositionInSquad(count);
			barrierSegments.Add(spawnedSegment);

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

void ABarrier::NotifyActorBeginOverlap(AActor* OtherActor) {

	UE_LOG(LogTemp, Warning, TEXT("Barrier overlapped with %s"), *(OtherActor->GetName()));

}

// TODO: destroy when no more segments are alive