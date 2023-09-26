// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
	: bulletType{ BulletType::PLAYER },		// By default, bullets will be of player type
	dir{},									// If no explicit value is specified, this defaults value to zero (vector [0,0,0] in this case)
	velocity{0.0f}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComponent");
	RootComponent = Mesh; // We need a RootComponent to have a base transform
	SetBulletMesh();

	// Bullets will overlap, blocking behaviour is not desirable
	// This overrides collision type for bullets, indicating that they should overlap instead of blocking colliding elements
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This will make the bullet automatically move in a direction on each render
	FVector location = GetActorLocation();
	location += DeltaTime * velocity * dir;
	SetActorLocation(location);

}

// This applies a static mesh to the bullet, to make it visible
void ABullet::SetBulletMesh(UStaticMesh* staticMesh, FString path, FVector scale) {

	const TCHAR* tpath;
	tpath = ABullet::defaultStaticMeshPath; // default route
	
	if (!Mesh) // No Mesh component
		return;

	// If static mesh is not specified in parameters, then we use a default one
	if (!staticMesh) {
		if (!path.IsEmpty())
			tpath = *path;

		// Fetch mesh from a given path
		auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(tpath);
		staticMesh = MeshAsset.Object;
	}
	
	if (staticMesh) {
		Mesh->SetStaticMesh(staticMesh);
		Mesh->SetRelativeScale3D(scale);
	}

}
