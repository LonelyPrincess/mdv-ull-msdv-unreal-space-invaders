

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barrier.generated.h"

DECLARE_DELEGATE_OneParam(FSegmentDestroyedDelegateSignature, int32);

UCLASS()
class SPACEINVADERS_API ABarrier : public AActor
{
	GENERATED_BODY()
	
public:

	// Event to identify that a segment has been lost
	FSegmentDestroyedDelegateSignature SegmentDestroyed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier config")
	int32 nRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier config")
	int32 nCols;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier config")
	TSubclassOf<class ABarrierSegment> segmentClass;

	UPROPERTY(VisibleAnywhere)
	TArray<class ABarrierSegment*> barrierSegments;

	UPROPERTY(VisibleAnywhere)
	int32 numSegments;

	// Sets default values for this actor's properties
	ABarrier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Collision event handler
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void OnSegmentDestroyed(int32 index);

	void SpawnBarrierSegments();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	static constexpr const TCHAR* defaultStaticMeshPath = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");

};
