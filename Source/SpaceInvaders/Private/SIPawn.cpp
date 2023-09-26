// Fill out your copyright notice in the Description page of Project Settings.


#include "SIPawn.h"

// Sets default values
ASIPawn::ASIPawn()
	: bFrozen{ false },		// Initialize class props
	bPause{ false }			// It's important that they appear in same order as in the ".h" file
{
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASIPawn::BeginPlay()
{
	Super::BeginPlay();

	// Generate a Bullet Template of the correct class
	if (bulletClass->IsChildOf<ABullet>())
		// If bullet class is an extension of ABullet, use it to instantiate the bullet
		bulletTemplate = NewObject<ABullet>(this, bulletClass);
	else
		// If specified class is not valid, we'd use ABullet base class by default
		bulletTemplate = NewObject<ABullet>(this);

	/*bulletTemplate->bulletType = BulletType::PLAYER;

	if (AudioComponent != nullptr && AudioShoot != nullptr) {
		AudioComponent->SetSound(AudioShoot);
	}*/

}


// Called every frame
void ASIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Inherit default controls from parent pawn class
	// Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Define actions associated to each custom axis and actions we defined in "Project settions > Input"
	PlayerInputComponent->BindAxis(TEXT("SIRight"), this, &ASIPawn::OnMove);
	PlayerInputComponent->BindAction(TEXT("SIFire"), IE_Pressed, this, &ASIPawn::OnFire);
	PlayerInputComponent->BindAction(TEXT("SIPause"), IE_Pressed, this, &ASIPawn::OnPause);

}

// Handler for horizontal movement axis
void ASIPawn::OnMove(float value) {

	if (bFrozen)
		return;

	// Get time ellapsed ever since the last render
	float deltaTime = GetWorld()->GetDeltaSeconds();

	// We use delta time to compensate movement speed on slower devices 
	//	(user will move larger distance on a single render in slow device, to compensate that user cannot make as many movements in the same time span as a faster device)
	float delta = velocity * value * deltaTime;

	// Horizontal movement will be based on Y axis
	FVector dir = FVector(0.0f, 1.0f, 0.0f);

	AddMovementInput(dir, delta);
}

// Handler for shoot event
void ASIPawn::OnFire() {

	if (bFrozen)
		return;

	/* FVector spawnLocation = GetActorLocation();
	FRotator spawnRotation = GetActorRotation();
	ABullet* spawnedBullet;
	bulletTemplate->velocity = bulletVelocity;
	bulletTemplate->dir = GetActorForwardVector();
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Template = bulletTemplate;
	spawnedBullet = Cast<ABullet>(GetWorld()->SpawnActor(bulletClass, &spawnLocation, &spawnRotation, spawnParameters));

	AudioComponent->Play(); */

}

// Handler for pause event
void ASIPawn::OnPause() {

	bPause = !bPause;

}
