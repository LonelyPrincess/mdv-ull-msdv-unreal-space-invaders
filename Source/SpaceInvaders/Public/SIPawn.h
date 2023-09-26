// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Bullet.h"

#include "SIPawn.generated.h"

UCLASS()
class SPACEINVADERS_API ASIPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:

	// EditAnywhere allows modifying value of this prop anywhere
	// BlueprintReadWrite allows seeing and modifying this value from BP editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defender Config")
	float velocity = 100.0f;

	// This property will be used to store the base class we'll use for bullets
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

protected:

	// Bindings for inputs
	void OnMove(float value);

	void OnFire();

	void OnPause();

};
