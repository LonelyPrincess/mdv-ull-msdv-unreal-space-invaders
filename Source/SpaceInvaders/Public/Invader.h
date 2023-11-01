// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

#include "Bullet.h"
#include "InvaderMovementComponent.h"

#include "Invader.generated.h"

UCLASS()
class SPACEINVADERS_API AInvader : public AActor
{
	GENERATED_BODY()

public:

	// Pointer to mesh that will be used to render the invader
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float bulletVelocity;

	// Audios to be played on different situations
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* AudioShoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* AudioExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* AudioJet;

	// Niagara system with the FX that will be executed when the ship explodes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* ExplosionFX;

	// Niagara system with the FX that will be executed when the ship shoots
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* ShootFX;

	// Define class that will be used to instance bullets 
	//	("TSubclassOf" guarantees one can only choose among those that inherit from "ABullet")
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class ABullet> bulletClass;

	UPROPERTY(EditAnywhere)
	class USceneComponent* BulletSpawnPoint;

	// Movement component to use on invader
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UInvaderMovementComponent* Movement;

	// Public Methods

	// Sets default values for this actor's properties
	AInvader();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Handler for overlap event
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable)
	void SetInvaderMesh(class UStaticMesh* staticMesh = nullptr, const FString path = TEXT(""), FVector scale = FVector(1.0f, 1.0f, 1.0f));

	UFUNCTION(BlueprintCallable)
	void Fire();

	// Allow setting the order of the invader inside the squad
	UFUNCTION(BlueprintCallable)
	void SetPositionInSquad(int32 index);

	UFUNCTION(BlueprintCallable)
	int32 GetPositionInSquad();

	// Method that indicates size of the mesh, to avoid squad elements from overlapping with each other
	UFUNCTION(BlueprintCallable)
	float GetBoundRadius();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// Pointer to class that will be used to spawn bullets
	UPROPERTY()
	class ABullet* bulletTemplate;

	UPROPERTY()
	class UAudioComponent* AudioComponent;

	// Position in squad will be a read-only value in editor: it can be set via C++ code alone
	UPROPERTY(VisibleInstanceOnly)
	int32 positionInSquad;

	UPROPERTY(VisibleInstanceOnly)
	float timeFromLastShot;

	UPROPERTY(VisibleAnywhere)
	FName leftSideTag;

	UPROPERTY(VisibleAnywhere)
	FName rightSideTag;

	UPROPERTY(VisibleAnywhere)
	FName downSideTag;

	bool bFrozen;
	bool bPause;

	// Timer to control waiting after destruction
	FTimerHandle timerHandle;

	// Method that's called when the invader dies
	UFUNCTION()
	void InvaderDestroyed();

	// Method that's called after the invader has been destroyed, after enough time has passed
	UFUNCTION()
	void PostInvaderDestroyed();

	UPROPERTY(VisibleInstanceOnly)
	FVector boundOrigin;

	UPROPERTY(VisibleInstanceOnly)
	float boundRadius;


	// Static literals of the class
	static constexpr const TCHAR* defaultStaticMeshName = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");
	static constexpr const TCHAR* leftSideTagString = TEXT("LeftLimit");
	static constexpr const TCHAR* rightSideTagString = TEXT("RightLimit");
	static constexpr const TCHAR* downSideTagString = TEXT("BottomLimit");

};