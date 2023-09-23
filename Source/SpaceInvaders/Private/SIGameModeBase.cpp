// Fill out your copyright notice in the Description page of Project Settings.


#include "SIGameModeBase.h"
#include "SIPawn.h"
#include "SIPlayerController.h"

// Constructor for GameMode class, which overrides default classes for pawn and player controller
ASIGameModeBase::ASIGameModeBase()
{

	DefaultPawnClass = ASIPawn::StaticClass();
	PlayerControllerClass = ASIPlayerController::StaticClass();

}