// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"

#include "SIPawn.generated.h"

UCLASS()
class SPACEINVADERS_API ASIPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:

	// Notes about UPROPERTY:
	// - EditAnywhere allows modifying value of the prop anywhere
	// - BlueprintReadWrite allows seeing and modifying the value of the property directly from BP editor
	// - Category can be set to organize properies in specific sections of the object details tab in the BP editor

	// Movement speed of the pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defender Config")
	float velocity = 100.0f;

	// This property will be used to store the base class we'll use for bullets
	// With "TSubclassOf" we indicate that only possible choice is for a classes inheriting from ABullet
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defender Config")
	TSubclassOf<class ABullet> bulletClass;

	// Sound that will run when user shoots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defender config")
	class USoundCue* AudioShoot;

	// Velocity of the player bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defender config")
	float bulletVelocity = 3000.0f;


private: 

	// Flag to indicate if pawn is in a frozen state (no moving and firing capabilities)
	bool bFrozen;

	// Flag that indicates if game is paused
	bool bPause;

	// We always need to add UPROPERTY annotation when declaring references to UE objects (ex.: actors)
	// This ensures that UE will handle them and prevent memory leaks, since UE will automatically delete it when not needed
	UPROPERTY()
	class ABullet* bulletTemplate;

	UPROPERTY()
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere)
	class USceneComponent* BulletSpawnPoint;

	static constexpr const TCHAR* defaultStaticMeshPath = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");

public:

	// Sets default values for this pawn's properties
	ASIPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// It could be possible to change the static mesh component during run time.
	UFUNCTION(BlueprintCallable)
	void SetStaticMesh(class UStaticMesh* staticMesh = nullptr, FString path = TEXT(""), FVector scale = FVector(1.0f, 1.0f, 1.0f));

protected:

	// Bindings for inputs
	void OnMove(float value);

	void OnFire();

	void OnPause();


public:

	// Points that will be awarded when pawn destroys an invader
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defender config")
	int32 pointsPerInvader;

	// Points that will be given when pawn destroys a full squad
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defender config")
	int32 pointsPerSquad;

	// Audios: Explossion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defender config")
	class USoundCue* AudioExplosion;

	// Niagara system with the FX that will be executed when the ship explodes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defender config")
	class UNiagaraSystem* ExplosionFX;

	// Niagara system with the FX that will be executed when the ship shoots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defender config")
	class UNiagaraSystem* ShootFX;

	// Overlap event handler
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Getters and Setters

	UFUNCTION(BlueprintCallable)
	int64 GetPoints();

	UFUNCTION(BlueprintCallable)
	int32 GetLifes();

protected:

	// Run effects involved in player destruction
	UFUNCTION(BlueprintCallable)
	void DestroyPlayer();

	// Event handler that will run after player has been destroyed
	UFUNCTION(BlueprintCallable)
	void PostPlayerDestroyed();

private:

	FTimerHandle timerHandle;

	// Reference to game mode instance
	UPROPERTY()
	class ASIGameModeBase* MyGameMode;

	// Player score
	int64 playerPoints;

	// Bindings to delegates
	void InvaderDestroyed(int32 id, bool killedByPlayer);
	void SquadDissolved(int32 val);
	void SquadSuccessful();

public:

	// Remaining lives
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defender config")
	int32 playerLifes;

};
