// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderMovementComponent.h"
#include "SIGameModeBase.h"

// Constructor to initialize class parameters
UInvaderMovementComponent::UInvaderMovementComponent()
	: state{ InvaderMovementType::STOP }
	, horizontalVelocity{ 1000.0f }
	, verticalVelocity{ 1000.0f }
	, descendingStep{ 100.0f }
	, freeJumpRadius{ 300.0f }
	, freeJumpVelocity{ 1000.0f }
	, deltaAlphaInterpolation{ 1.0f / 30.0f }
	, numberOfTargetPoints{ 5 }
	, previousState{ InvaderMovementType::STOP }
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Method called as soon as actor becomes active in scene
void UInvaderMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Store reference to game mode
	UWorld* TheWorld = GetWorld();
	if (TheWorld) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		MyGameMode = Cast<ASIGameModeBase>(GameMode);
	}

	// Apply random angle so the invader is looking on a random direction at the start
	finalAngle = FMath::RandRange(-30.0f, 30.0f);

}

// Generate a sequence of geometric transformation to perform a circular trajectory
void UInvaderMovementComponent::GenerateTargetPoints() {

	// Obtain actor to which the component is linked to, so we can fetch its location
	AActor* Parent = GetOwner();
	FTransform initialTransform;
	FVector initialLocation;
	FVector initialScale;

	FQuat initialQuaternion;
	FVector forward;

	if (!Parent) {
		numberOfTargetPoints = 0;
		return;
	}

	initialTransform = Parent->GetActorTransform();
	initialLocation = initialTransform.GetLocation();
	initialScale = initialTransform.GetScale3D();
	initialQuaternion = initialTransform.GetRotation();
	forward = Parent->GetActorForwardVector();

	// The first stage movement is a circle
	// Calculate center of the circle from actor location
	float radio = freeJumpRadius;
	FVector center = initialLocation;

	center.X += radio;

	if (numberOfTargetPoints > 0) {
		float theta = 0.0f; // Theta is the angle that will be applied on each waypoint
		float deltaTheta = 2 * PI / numberOfTargetPoints;

		// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("X is %g Y is %g"), initialLocation.X, initialLocation.Y));

		FVector newLocation = initialLocation;
		FRotator rotation;
		FTransform newTransform = initialTransform;
		for (int32 i = 0; i < numberOfTargetPoints; i++) {

			float pc = FMath::Cos(theta);
			float ps = FMath::Sin(theta);

			newLocation.X = center.X - radio * FMath::Cos(theta);
			newLocation.Y = center.Y + radio * FMath::Sin(theta);
			newTransform.SetLocation(newLocation);

			// Change the rotation of the actor to follow the tangent of the circle
			if (i != (numberOfTargetPoints - 1)) {
				// FRotator requires angles in degrees!
				// Rotation is in Yaw. The following angle makes the invader to follow the tangent
				rotation = FRotator(0.0f, -(theta * 180.0f / PI) - 90, 0.0f);
				FQuat newQuaternion = rotation.Quaternion() * initialQuaternion;
				newTransform.SetRotation(newQuaternion);
			}
			else // Last point set the invader with the initial transformation
				newTransform.SetRotation(initialQuaternion); // Last transformation 
			targetPoints.Add(newTransform);
			theta += deltaTheta;
		}

	}

}

// The purpose of having target points is getting the transformations to apply in the trajectory through interpolation
// The following function performs the interpolation
FTransform UInvaderMovementComponent::InterpolateWithTargetPoints(FTransform origin, float fraction) {

	FVector originLocation = origin.GetLocation();
	FQuat originRotation = origin.GetRotation();
	FVector targetLocation;
	FQuat targetRotation;

	if (currentTargetPoint >= 0 && currentTargetPoint < numberOfTargetPoints) {
		targetLocation = targetPoints[currentTargetPoint].GetLocation();
		targetRotation = targetPoints[currentTargetPoint].GetRotation();
	}
	else
		return origin;

	// Location interporlation for vectors (uses linear interpolation)
	FVector newLocation = UKismetMathLibrary::VLerp(originLocation, targetLocation, fraction);

	// Spherical interpolation for quaterions
	FQuat newRotation = FQuat::Slerp(originRotation, targetRotation, fraction);

	FTransform newTransform = origin;
	newTransform.SetLocation(newLocation);
	newTransform.SetRotation(newRotation);
	return newTransform;

}

// Method called on every render
void UInvaderMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Parent = GetOwner(); // Parent is the actor who owns this component.

	if (!Parent)
		return;

	float deltaHorizontal = horizontalVelocity * DeltaTime; // Increment in horizontal and vertical dimensions given DeltaTime and parameterized velocities
	float deltaVertical = verticalVelocity * DeltaTime;

	float deltaX = 0.0f; // These deltas determine the change of the actor position
	float deltaY = 0.0f;

	// deltaX and deltaY are calculated differently for each movement type
	// The movement type is in the public variable state
	// previousState is updated
	switch (state) {

		// If movement type is stop, we don't apply any changes to rotation
		case InvaderMovementType::STOP:
			deltaX = 0.0f;
			deltaY = 0.0f;
			previousState = InvaderMovementType::STOP;
			break;

		// Turn right
		case InvaderMovementType::RIGHT:

			deltaX = 0.0f;
			deltaY = deltaHorizontal;

			previousState = InvaderMovementType::RIGHT;
			break;

		// Turn left
		case InvaderMovementType::LEFT:

			deltaX = 0.0f;
			deltaY = -deltaHorizontal;
			previousState = InvaderMovementType::LEFT;
			break;

		// Down movement: this is an automatic movement that has to finish automatically
		// It is based on an internal variable, descendinfProgress, that is updated.
		case InvaderMovementType::DOWN:

			// If invader was not descending yet, set progress counter to initialize descending movement
			if (previousState != InvaderMovementType::DOWN) {
				descendingProgress = 0.0f; // This means that the down phase is starting
			}

			// If descending progress exceeds the distance we configured in step param, we must stop invader from going down anymore
			if (descendingProgress > descendingStep) {
				deltaVertical = 0.0f; // This means that the down phase stops

				// Trigger event to indicate squad has gone down as much as possible
				MyGameMode->SquadFinishesDown.ExecuteIfBound();
			}

			deltaX = -deltaVertical;
			deltaY = 0.0f;
			descendingProgress += deltaVertical;
			previousState = InvaderMovementType::DOWN;
			break;

		// Free jump movement: this is an automatic complex movement.
		case InvaderMovementType::FREEJUMP:

			deltaX = 0.0f; // This movement is not based on deltaX, deltaY, but in general transformations
			deltaY = 0.0f;

			// If invader has just got in free jump state, generate waypoints to choose how it'll move
			if (previousState != InvaderMovementType::FREEJUMP) { // First time we enter in FREEJUMP
				GenerateTargetPoints();
				currentTargetPoint = 0;
				if (numberOfTargetPoints > 0) {
					originTransform = Parent->GetActorTransform(); // First originTransform for interpolation is actor transform
					alphaInterpolation = 0.0f;
				}

				previousState = InvaderMovementType::FREEJUMP;
			}

			// Now the movement is programatically defined.
			// There are two stages:
			// First stage: an automatic movement defined by a sequence of target transforms
			// currentTargetPoint is the index of the current transform
			if (currentTargetPoint < numberOfTargetPoints) {
				// New transform calculated by interpolation between current and currentTargetPoint.
				FTransform newtransform = InterpolateWithTargetPoints(originTransform, alphaInterpolation);
				// The actor receive the new transform					
				Parent->SetActorTransform(newtransform);

				alphaInterpolation += deltaAlphaInterpolation;
				if (alphaInterpolation > 1.0f) { // target has been reached with interpolation
					++currentTargetPoint;
					alphaInterpolation = 0.0f; // To start a new segment of linear interpolation

					if (currentTargetPoint < numberOfTargetPoints) // new originTransform is previous target
						originTransform = this->targetPoints[currentTargetPoint - 1];
					// If this was the last target we add a random rotation that it will define the direction of the movement in the second stage of the free jump
					else {
						Parent->AddActorLocalRotation(FRotator(0.0f, finalAngle, 0.0f));
					}
				}
			}

			// Second stage: the actor is simply moved in the forward direction after having gone through all waypoints
			else {
				FVector parentLocation = Parent->GetActorLocation();
				FVector forward = Parent->GetActorForwardVector();
				FVector right = Parent->GetActorRightVector();
				parentLocation += freeJumpVelocity * DeltaTime * forward;

				Parent->SetActorLocation(parentLocation);
			}
	}

	// Apply calculated deltaX deltaY for those movements based on them (right, left)
	if (Parent && state != InvaderMovementType::FREEJUMP) {

		FVector parentLocation = Parent->GetActorLocation();
		parentLocation.X += deltaX;
		parentLocation.Y += deltaY;
		Parent->SetActorLocation(parentLocation);

	}
}
