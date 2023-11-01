

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barrier.generated.h"

UCLASS()
class SPACEINVADERS_API ABarrier : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier config")
	int32 nRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier config")
	int32 nCols;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier config")
	TSubclassOf<class ABarrierSegment> segmentClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<class ABarrierSegment*> barrierSegments;

	// Sets default values for this actor's properties
	ABarrier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Collision event handler
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	static constexpr const TCHAR* defaultStaticMeshPath = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");

};
