// Fill out your copyright notice in the Description page of Project Settings.


#include "SIPawn.h"

#include "SIGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Bullet.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ASIPawn::ASIPawn()
	: velocity{ 1000 },
	AudioShoot{},
	bulletVelocity{ 3000 },
	bFrozen{ false },			// Initialize class props
	bPause{ false },			// It's important that they appear in same order as in the ".h" file
	pointsPerInvader{ 100 },
	pointsPerSquad{ 1000 },
	AudioExplosion{},
	MyGameMode{},
	playerPoints{ 0 },
	playerLifes{ 5 }
{
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetStaticMesh(); // Default mesh (SetStaticMesh with no arguments)

	// Initialize audio component that will be added to current actor and follow them around
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	// Add scene component which can be used to customize the position in which bullets will be spawned
	BulletSpawnPointComponent = CreateDefaultSubobject<USceneComponent>("BulletSpawnPoint");
	BulletSpawnPointComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

}

// Set a static mesh
void ASIPawn::SetStaticMesh(UStaticMesh* staticMesh, FString path, FVector scale) {
	UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	const TCHAR* tpath;
	tpath = ASIPawn::defaultStaticMeshPath; // default route

	if (!Mesh) // No Mesh component
		return;

	if (!staticMesh) {
		if (!path.IsEmpty())
			tpath = *path;
		auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(tpath);
		staticMesh = MeshAsset.Object;
	}

	if (staticMesh) {
		Mesh->SetStaticMesh(staticMesh);
		Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

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

	// "GetWorld" will give us a reference to the level itself, so we can use it to fetch the game mode
	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		MyGameMode = Cast<ASIGameModeBase>(GameMode);

		// Subscribe to events
		if (MyGameMode) {
			MyGameMode->InvaderDestroyed.AddUObject(this, &ASIPawn::InvaderDestroyed);
			MyGameMode->SquadSuccessful.BindUObject(this, &ASIPawn::SquadSuccessful);
			MyGameMode->NewSquad.AddUObject(this, &ASIPawn::SquadDissolved);
		}
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

	FVector spawnLocation = BulletSpawnPointComponent->GetComponentLocation();
	FRotator spawnRotation = BulletSpawnPointComponent->GetComponentRotation();
	ABullet* spawnedBullet;
	bulletTemplate->velocity = bulletVelocity;
	bulletTemplate->dir = GetActorForwardVector();

	// Spawn bullets based on the selected template
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Template = bulletTemplate;
	spawnedBullet = Cast<ABullet>(GetWorld()->SpawnActor(bulletClass, &spawnLocation, &spawnRotation, spawnParameters));

	// Assign sound that will be played on shoot
	if (AudioComponent != nullptr && AudioShoot != nullptr) {
		AudioComponent->SetSound(AudioShoot);
	}

	// Reproduce a sound when the user shoots
	AudioComponent->Play();

}

// Handler for pause event
void ASIPawn::OnPause() {

	UE_LOG(LogTemp, Display, TEXT("Pause event triggered!"));

	bPause = !bPause;

}

// Return current score
int64 ASIPawn::GetPoints() {
	return this->playerPoints;

}

// Return remaining lives of the user
int32 ASIPawn::GetLifes() {
	return this->playerLifes;

}

// Handle collissions
void ASIPawn::NotifyActorBeginOverlap(AActor* OtherActor) {

	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%s collided with player"), *(OtherActor->GetName())));

	if (!bFrozen) {
		// User will die if collides with enemy bullet
		if (OtherActor->IsA(ABullet::StaticClass())) {
			ABullet* bullet = Cast<ABullet>(OtherActor);
			UE_LOG(LogTemp, Display, TEXT("Collided with bullet"));
			if (bullet->bulletType == BulletType::INVADER) {
				UE_LOG(LogTemp, Display, TEXT("Collided with enemy bullet"));
				OtherActor->Destroy();
				DestroyPlayer();
			}
		}

		// User will also die if collides directly with an invader
		if (OtherActor->IsA(AInvader::StaticClass())) {
			UE_LOG(LogTemp, Display, TEXT("Collided with invader"));
			OtherActor->Destroy();
			DestroyPlayer();
		}
	}

}

// This triggers some effects when the player dies
void ASIPawn::DestroyPlayer() {
	UWorld* TheWorld;
	TheWorld = GetWorld();

	UE_LOG(LogTemp, Display, TEXT("Player was hit!"));
	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Player should die"));

	if (TheWorld) {

		bFrozen = true; // Pawn can't move or fire while being destroyed

		// Decrease amount of lives of the user
		--this->playerLifes;

		// Trigger visual effect on explosion
		if (ExplosionFX != nullptr) {
			UNiagaraFunctionLibrary::SpawnSystemAttached(ExplosionFX, RootComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
		}

		// Hide Static Mesh Component
		UStaticMeshComponent* LocalMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (LocalMeshComponent != nullptr) {
			LocalMeshComponent->SetVisibility(false);
		}

		// Play explosion sound
		if (AudioComponent != nullptr && AudioExplosion != nullptr) {
			AudioComponent->SetSound(AudioExplosion);
			AudioComponent->Play();
		}

		// Wait a few seconds before removing item from scene
		TheWorld->GetTimerManager().SetTimer(timerHandle, this, &ASIPawn::PostPlayerDestroyed, 3.0f, false);
	}
}

// Function that's called after user dies
void ASIPawn::PostPlayerDestroyed() {

	// If user has no more lives, trigger event
	if (this->playerLifes == 0) {
		UE_LOG(LogTemp, Display, TEXT("Player is dead"));
		if (MyGameMode)
			MyGameMode->PlayerZeroLifes.ExecuteIfBound(this->playerPoints);
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Player will come back to life"));

	// If user still had some lives left, restore the mesh and allow the user to keep moving
	UStaticMeshComponent* LocalMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	// Show Static Mesh Component
	if (LocalMeshComponent != nullptr) {
		LocalMeshComponent->SetVisibility(true);
	}
	// Unfrozing
	bFrozen = false;

}

// TODO: There seems to be a bug here! Points are increasing when the invader self-destroys
// We should only add up to the score when the player killed it, it's weird for it to happen otherwise
// This can probably be fixed by adding some parameter to this event, to indicate the cause of destruction
// Either that, or have separate events for both cases (invader killed by user, invader destroyed for other reasons)

// Delegate responses:
void ASIPawn::InvaderDestroyed(int32 id) {
	this->playerPoints += this->pointsPerInvader;
}

void ASIPawn::SquadSuccessful() {
	DestroyPlayer();

	// Regenerate a new squad after the previous one has successfully reached the player
	if (MyGameMode)
		MyGameMode->NewSquad.Broadcast(this->playerLifes);
}

void ASIPawn::SquadDissolved(int32 val) {
	this->playerPoints += this->pointsPerSquad;
}
