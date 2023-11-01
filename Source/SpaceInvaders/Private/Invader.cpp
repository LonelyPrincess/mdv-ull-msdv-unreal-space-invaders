// Fill out your copyright notice in the Description page of Project Settings.


#include "Invader.h"
#include "SIGameModeBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AInvader::AInvader()
	: fireRate{ 0.0001f }
	, bulletVelocity{ 3000.0f }
	, bulletClass{ ABullet::StaticClass() }
	, positionInSquad{}
	, timeFromLastShot{}
	, leftSideTag{ FName(AInvader::leftSideTagString) }
	, rightSideTag{ FName(AInvader::rightSideTagString) }
	, downSideTag{ FName(AInvader::downSideTagString) }
	, bFrozen{ false }
	, bPause{ false }
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add static mesh component to actor, and convert it to root node in component tree
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComponent");
	RootComponent = Mesh; // We need a RootComponent to have a base transform

	SetInvaderMesh();

	// Add audio component to play certain sounds on invader actions
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	// Add scene component which can be used to customize the position in which bullets will be spawned
	BulletSpawnPoint = CreateDefaultSubobject<USceneComponent>("BulletSpawnPoint");
	BulletSpawnPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	// Add movement component to the invader
	// We do not use "AttachTo" in this case because this ain't a scene component, but an actor component which no transform element
	Movement = CreateDefaultSubobject<UInvaderMovementComponent>("InvaderMoveComponent");
	AddOwnedComponent(Movement); // Because UInvaderMovementComponent is only an Actor Component and not a Scene Component can't Attach To.
}

// Called when the game starts or when spawned
void AInvader::BeginPlay()
{
	Super::BeginPlay();

	// Generate a Bullet Template of the correct class
	if (bulletClass != nullptr && bulletClass->IsChildOf<ABullet>())
		bulletTemplate = NewObject<ABullet>(this, bulletClass->GetFName(), RF_NoFlags, bulletClass.GetDefaultObject());
	else {
		UE_LOG(LogTemp, Warning, TEXT("Selected bullet class is either empty or invalid, so default class will be used..."));

		// If specified class is not valid, we'd use ABullet base class by default
		bulletClass = ABullet::StaticClass();
		bulletTemplate = NewObject<ABullet>();
	}

	bulletTemplate->bulletType = BulletType::INVADER;

}

// Called every frame
void AInvader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If invader is in frozen state, change its movement type to stopped
	if (bFrozen) {
		Movement->state = InvaderMovementType::STOP;
		return;
	}

	// Calculate time ellapsed since last shot
	this->timeFromLastShot += DeltaTime;

	// Randomly trigger a new shoot (higher chance of firing after a long time since last shot)
	float val = FMath::RandRange(0.0f, 1.0f);
	if (!bFrozen && val < (1.0 - FMath::Exp(-fireRate * this->timeFromLastShot)))
		Fire();

}

// Assign mesh to invader actor
void AInvader::SetInvaderMesh(UStaticMesh* newStaticMesh, const FString path, FVector scale) {
	const TCHAR* tpath;
	tpath = AInvader::defaultStaticMeshName; // default route

	if (!Mesh) // No Mesh component
		return;

	// If no mesh was selected, apply one by default
	if (!newStaticMesh) {
		if (!path.IsEmpty())
			tpath = *path;
		auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(tpath);
		newStaticMesh = MeshAsset.Object;
	}

	// Assign the mesh asset to our static mesh component
	if (newStaticMesh) {
		Mesh->SetStaticMesh(newStaticMesh);
		Mesh->SetRelativeScale3D(scale);
		FBoxSphereBounds meshBounds = Mesh->Bounds;
		boundOrigin = meshBounds.Origin;
		boundRadius = meshBounds.SphereRadius;
	}
}

// Method to perform a new shot
void AInvader::Fire() {

	FVector spawnLocation = BulletSpawnPoint->GetComponentLocation();
	FRotator spawnRotation = BulletSpawnPoint->GetComponentRotation();
	ABullet* spawnedBullet;

	if (this->bulletTemplate) {
		this->bulletTemplate->velocity = bulletVelocity;
		this->bulletTemplate->dir = GetActorForwardVector();

		// Spawn bullet on invader location
		FActorSpawnParameters spawnParameters;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParameters.Template = this->bulletTemplate;
		spawnedBullet = (ABullet*)GetWorld()->SpawnActor<ABullet>(spawnLocation, spawnRotation, spawnParameters);

		// Trigger visual effect on explosion
		if (ShootFX != nullptr) {
			UNiagaraFunctionLibrary::SpawnSystemAttached(ShootFX, BulletSpawnPoint, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
		}

		// If SFX is defined for shoot action, play it
		if (AudioComponent != nullptr && AudioShoot != nullptr) {
			AudioComponent->SetSound(AudioShoot);
			AudioComponent->Play();
		}

		// Reset counter with time since last shot
		this->timeFromLastShot = 0.0f;
	}
}

void AInvader::NotifyActorBeginOverlap(AActor* OtherActor) {

	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%s entered me"), *(OtherActor->GetName())));

	FName actorTag;

	// If invader is on frozen state (either dead or game is paused), do nothing
	if (bFrozen)
		return;

	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr) {
		UInvaderMovementComponent* imc = (UInvaderMovementComponent*)this->GetComponentByClass(UInvaderMovementComponent::StaticClass());
		bool bFreeJump = imc->state == InvaderMovementType::FREEJUMP;
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ASIGameModeBase* MyGameMode = Cast<ASIGameModeBase>(GameMode);
		UClass* otherActorClass = OtherActor->GetClass();

		// Handle collisions with bullet items
		if (OtherActor->IsA(ABullet::StaticClass())) {
			ABullet* bullet = Cast<ABullet>(OtherActor);

			// Invader dies if they collide with a bullet shot by the player
			if (bullet->bulletType == BulletType::PLAYER) {
				// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Invader %d killed"), this->positionInSquad));

				OtherActor->Destroy();
				// Trigger event indicating invader has been killed
				MyGameMode->InvaderDestroyed.Broadcast(this->positionInSquad, true);
				InvaderDestroyed();
				return;
			}
			else
				return; // It's an invader bullet, so it has to be ignored
		}

		// Overlap with other invaders is ignored
		if (OtherActor->IsA(AInvader::StaticClass()))
			return;

		// Overlap with anything in freejump (except invaders and their own bullets) causes a silent destroy
		if (bFreeJump) {
			MyGameMode->InvaderDestroyed.Broadcast(this->positionInSquad, false);
			Destroy();
			return;
		}

		// Trigger events when invader collides with any of the map bounds (left, right or bottom)
		if (OtherActor->ActorHasTag(leftSideTag) && !bFreeJump) {
			MyGameMode->SquadOnLeftSide.ExecuteIfBound();
		}
		else if (OtherActor->ActorHasTag(rightSideTag) && !bFreeJump) {
			MyGameMode->SquadOnRightSide.ExecuteIfBound();
		}
		else if (OtherActor->ActorHasTag(downSideTag) && !bFreeJump) {
			MyGameMode->SquadSuccessful.ExecuteIfBound(); // Squad wins!
		}
	}

}

// Function that's called when invader collides with a bullet from the player, to trigger their death
void AInvader::InvaderDestroyed() {
	UWorld* TheWorld;
	TheWorld = GetWorld(); // To get utilities as the timer.

	if (TheWorld) {
		bFrozen = true; // Invader can't move or fire while being destroyed

		UStaticMeshComponent* LocalMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

		// Trigger visual effect on explosion
		if (ExplosionFX != nullptr) {
			UNiagaraFunctionLibrary::SpawnSystemAttached(ExplosionFX, RootComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
		}

		// Hide static mesh component so object it's not visible anymore
		if (LocalMeshComponent != nullptr) {
			LocalMeshComponent->SetVisibility(false);
		}

		// Play sound of explosion to indicate invader's destruction
		if (AudioComponent != nullptr && AudioExplosion != nullptr) {
			AudioComponent->SetSound(AudioExplosion);
			AudioComponent->Play();
		}

		// Wait some time (2 secs) before triggering the actual destruction of the actor object
		TheWorld->GetTimerManager().SetTimer(timerHandle, this, &AInvader::PostInvaderDestroyed, 2.0f, false);
	}
}

// Method that triggers destruction of the object to completely remove it from the scene
void AInvader::PostInvaderDestroyed() {
	Destroy();
}

// Getter & Setter methods to access private props from the outside
void AInvader::SetPositionInSquad(int32 index)
{
	this->positionInSquad = index;
}

int32 AInvader::GetPositionInSquad()
{
	return int32(this->positionInSquad);
}

float AInvader::GetBoundRadius() {
	return this->boundRadius;
}

void AInvader::IncreaseFireRate(float multiplier) {
	this->fireRate *= multiplier;
}