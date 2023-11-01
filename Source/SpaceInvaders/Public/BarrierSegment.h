

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BarrierSegment.generated.h"

UCLASS()
class SPACEINVADERS_API ABarrierSegment : public AActor
{
	GENERATED_BODY()
	
public:	

	// Static mesh for a barrier segment
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* BarrierSegmentMesh;

	// Niagara system with the FX that will be executed when the ship explodes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* BarrierBreakFX;

	// Sets default values for this actor's properties
	ABarrierSegment();

	// Collision event handler
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetBoundRadius();

private:

	UPROPERTY(VisibleInstanceOnly)
	FVector boundOrigin;

	UPROPERTY(VisibleInstanceOnly)
	float boundRadius;

	static constexpr const TCHAR* staticMeshPath = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");

};
