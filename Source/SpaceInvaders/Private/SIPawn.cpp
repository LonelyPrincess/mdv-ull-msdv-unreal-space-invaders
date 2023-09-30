// Fill out your copyright notice in the Description page of Project Settings.


#include "SIPawn.h"

// Sets default values
ASIPawn::ASIPawn()
	: bFrozen{ false },		// Initialize class props
	bPause{ false }			// It's important that they appear in same order as in the ".h" file
{
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize audio component that will be added to current actor and follow them around
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

}

// Called when the game starts or when spawned
void ASIPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Generate a Bullet Template of the correct class
	if (bulletClass != nullptr && bulletClass->IsChildOf<ABullet>())
		// If bullet class is an extension of ABullet, use it to instantiate the bullet
		bulletTemplate = NewObject<ABullet>(this, bulletClass);
	else {
		UE_LOG(LogTemp, Warning, TEXT("Selected bullet class is either empty or invalid, so default class will be used..."));

		// If specified class is not valid, we'd use ABullet base class by default
		bulletClass = ABullet::StaticClass();
		bulletTemplate = NewObject<ABullet>(this);
	}

	bulletTemplate->bulletType = BulletType::PLAYER;

	// Assign sound that will be played on shoot
	if (AudioComponent != nullptr && AudioShoot != nullptr) {
		AudioComponent->SetSound(AudioShoot);
	}

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

	UE_LOG(LogTemp, Display, TEXT("Fire event triggered!"));

	if (bFrozen)
		return;

	FVector spawnLocation = GetActorLocation();
	FRotator spawnRotation = GetActorRotation();
	ABullet* spawnedBullet;
	bulletTemplate->velocity = bulletVelocity;
	bulletTemplate->dir = GetActorForwardVector();

	// Spawn bullets based on the selected template
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Template = bulletTemplate;
	spawnedBullet = Cast<ABullet>(GetWorld()->SpawnActor(bulletClass, &spawnLocation, &spawnRotation, spawnParameters));

	// Reproduce a sound when the user shoots
	AudioComponent->Play();

}

// Handler for pause event
void ASIPawn::OnPause() {

	UE_LOG(LogTemp, Display, TEXT("Pause event triggered!"));

	bPause = !bPause;

}
