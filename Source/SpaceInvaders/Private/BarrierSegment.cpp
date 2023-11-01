


#include "BarrierSegment.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Barrier overlapped with %s"), *(OtherActor->GetName()));
	if (OtherActor->GetClass()->IsChildOf(ABarrierSegment::StaticClass())) {
		return;
	}

	/*
	if (BarrierBreakFX != nullptr) {
		UNiagaraFunctionLibrary::SpawnSystemAttached(BarrierBreakFX, RootComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
	}

	// Hide static mesh component so object it's not visible anymore
	if (LocalMeshComponent != nullptr) {
		LocalMeshComponent->SetVisibility(false);
	}

	UWorld* TheWorld = GetWorld();

	// Wait some time (2 secs) before triggering the actual destruction of the actor object
	TheWorld->GetTimerManager().SetTimer(timerHandle, this, &ABarrier::Destroy, 1.0f, false);
	*/
}

float ABarrierSegment::GetBoundRadius() {
	return this->boundRadius;
}