// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ASIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ASIPlayerController();

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TArray<class AActor*> m_cameras;		// Array to store available cameras on scene
	FName m_mainCameraTag;					// Variable to store tag which will be used to fetch cameras from the code

};
