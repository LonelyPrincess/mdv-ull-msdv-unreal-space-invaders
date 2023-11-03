#include "Barrier.h"
#include "BarrierSegment.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ABarrierSegment::ABarrierSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Here we add a static mesh component to the barrier segments's actor component hierarchy
	BarrierSegmentMesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = BarrierSegmentMesh;

	// We'll apply cube mesh by default
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(ABarrierSegment::staticMeshPath);
	UStaticMesh* staticMesh = MeshAsset.Object;
	BarrierSegmentMesh->SetStaticMesh(MeshAsset.Object);

	BarrierSegmentMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	FBoxSphereBounds meshBounds = BarrierSegmentMesh->Bounds;
	boundOrigin = meshBounds.Origin;
	boundRadius = meshBounds.SphereRadius;

}

// Called when the game starts or when spawned
void ABarrierSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarrierSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABarrierSegment::NotifyActorBeginOverlap(AActor* OtherActor) {

	UE_LOG(LogTemp, Warning, TEXT("Barrier segment %i overlapped with %s"), this->segmentIndex, *(OtherActor->GetName()));
	if (OtherActor->GetClass()->IsChildOf(ABarrierSegment::StaticClass())) {
		return;
	}

	// Disable collisions so this doesn't run again
	SetActorEnableCollision(false);

	if (BarrierBreakFX != nullptr) {
		UNiagaraFunctionLibrary::SpawnSystemAttached(BarrierBreakFX, RootComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
	}

	// Hide static mesh component so object it's not visible anymore
	if (BarrierSegmentMesh != nullptr) {
		BarrierSegmentMesh->SetVisibility(false);
	}

	UWorld* TheWorld = GetWorld();
	TheWorld->GetTimerManager().SetTimer(timerHandle, this, &ABarrierSegment::SelfDestruct, 1.0f, false);

}

float ABarrierSegment::GetBoundRadius() {
	return this->boundRadius;
}

void ABarrierSegment::SetSegmentIndex(int32 index) {
	UE_LOG(LogTemp, Warning, TEXT("Barrier segment assigned index %i"), index);
	this->segmentIndex = index;
}

void ABarrierSegment::SetParent(ABarrier* barrier) {
	this->parent = barrier;
}

void ABarrierSegment::SelfDestruct() {
	UE_LOG(LogTemp, Warning, TEXT("Barrier segment %i is running self-destruct!"), this->segmentIndex);
	if (parent != nullptr) {
		parent->SegmentDestroyed.ExecuteIfBound(segmentIndex);
	}

	Destroy();
}