// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

// Enum with bullet types
UENUM(BlueprintType)
enum class BulletType : uint8 {
	PLAYER = 0 UMETA(DisplayName = "Player Type"),
	INVADER = 1 UMETA(DisplayName = "Invader Type")
};

UCLASS()
class SPACEINVADERS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
