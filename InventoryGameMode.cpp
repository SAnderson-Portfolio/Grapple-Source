// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "InventoryGameMode.h"
#include "InventoryHUD.h"
#include "InventoryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
void AInventoryGameMode::BeginPlay()
{
	//When the game starts apply hud to screen
	ApplyHUDChanges();
}

AInventoryGameMode::AInventoryGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AInventoryHUD::StaticClass();
	//Set default HUD to ingame state
	HUDState = EHUDState::HS_Ingame;
}

void AInventoryGameMode::ApplyHUDChanges()
{
	//Remove previous HUD and replace with new one

	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
	}

	switch (HUDState)
	{
	case EHUDState::HS_Ingame:
		ApplyHUD(IngameHUDClass, false, false);
		break;
	case EHUDState::HS_Inventory:
		ApplyHUD(InventoryHUDClass, true, true);
		break;
	case EHUDState::HS_Shop_General:
		ApplyHUD(ShopGeneralHUDClass, true, true);
		break;
	default:
		ApplyHUD(IngameHUDClass, false, false);
	}
}

uint8 AInventoryGameMode::GetHUDState()
{
	return HUDState;
}

void AInventoryGameMode::ChangeHUDState(uint8 NewState)
{
	HUDState = NewState;
	ApplyHUDChanges();
}

bool AInventoryGameMode::ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents)
{
	//Get a reference to the player and the controller
	AInventoryCharacter* MyCharacter = Cast<AInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	//Null check widget

	if (WidgetToApply != nullptr)
	{
		//Set mouse vis and click events
		MyController->bShowMouseCursor = bShowMouseCursor;
		MyController->bEnableClickEvents = EnableClickEvents;

		//Create widget
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
			return true;
		}
		else return false;
	}
	else return false;
}
