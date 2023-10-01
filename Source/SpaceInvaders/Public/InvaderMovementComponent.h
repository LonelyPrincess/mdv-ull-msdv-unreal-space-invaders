// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InvaderMovementComponent.generated.h"

// Enum with all supported movement types
//	(it's recommended to map all enum status to an integer of 8 bits, like this; otherwise they may not work in blueprints)
//	(naming convention specifies that all enum types shall start with "E", like "EInvaderMovementType")
UENUM(BlueprintType)
enum class InvaderMovementType : uint8 {

	STOP = 0 UMETA(DisplayName = "Stopped"),
	RIGHT = 1 UMETA(DisplayName = "Right"),
	LEFT = 2 UMETA(DisplayName = "Left"),
	DOWN = 3 UMETA(DisplayName = "Down"),
	FREEJUMP = 4 UMETA(DisplayName = "Free Jump")

};

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API UInvaderMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
public:

	// Sets default values for this component's properties
	UInvaderMovementComponent();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Invader Movement")
	float horizontalVelocity;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Invader Movement")
	float verticalVelocity;

	// Property to indicate current state of the object, which movement type is being applied
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Invader Movement")
	InvaderMovementType state;


	// Down movement parameters:
	// Movement to go down will be done on fixed steps based on the value of this property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invader Movement")
	float descendingStep; // Length of the descending step


	// Free jump parameters:
	// List of points that will create a circuit to determine how the invader will move (waypoints)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invader Movement")
	TArray<FTransform> targetPoints; // The movement will be composed by interpolation using a sequence of reference poses stored in this array.

	// Number of waypoints in the sequence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invader Movement")
	int32 numberOfTargetPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invader Movement")
	float freeJumpRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invader Movement")
	float freeJumpVelocity; // Velocity in the first stage of the free jump

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invader Movement")
	float deltaAlphaInterpolation; // Number of updates between two target points

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	// Generate waypoints for current invader based on their current location
	UFUNCTION(BlueprintCallable)
	void GenerateTargetPoints();

	UFUNCTION(BlueprintCallable)
	FTransform InterpolateWithTargetPoints(FTransform transform, float covered);

private:

	// Store ref to game mode to be able to trigger events
	class ASIGameModeBase* MyGameMode;

	// This stores the previous movement direction of the invader, so we can resume it later
	//	(ex.: if user was moving right and then descends, this variable can be used to keep moving right once descend has finished)
	InvaderMovementType previousState; // Store state in previous frame (to know when a state is beginning)

	// Down movement state variables:
	float descendingProgress = 0.0f; // Store progress in the Down state

	// Free jump movement state variables:
	FTransform originTransform;
	float alphaInterpolation;
	int32 currentTargetPoint = 0; // It stores the index of the first reference pose (the other is currentTargetPoint+1)

	// Random angle which will determine direction in which the invader will attack
	float finalAngle; // Orientation of the invader to start the final attack

};
