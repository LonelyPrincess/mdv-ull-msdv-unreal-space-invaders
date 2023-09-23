// Fill out your copyright notice in the Description page of Project Settings.


#include "SIPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Constructor where we assign initial size for cameras array
ASIPlayerController::ASIPlayerController()
	: m_mainCameraTag("MainCamera")			// This assigns default value to main camera tag prop
{
	m_cameras.Reserve(5);
}

// Begin play event is triggered when controller is ready to use
void ASIPlayerController::BeginPlay() {
	// Fetch all cameras available on scene and store them in array
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), m_mainCameraTag, m_cameras);

	// Set the first camera tagged as "MainCamera" as the view target
	if (m_cameras.Num() > 0)
		this->SetViewTarget(m_cameras[0]);
}
