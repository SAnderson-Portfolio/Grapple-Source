// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InventoryGameMode.generated.h"

UCLASS(minimalapi)
class AInventoryGameMode : public AGameModeBase
{
	GENERATED_BODY()

		virtual void BeginPlay() override;
public:
	AInventoryGameMode();

	enum EHUDState : uint8
	{
		HS_Ingame,
		HS_Inventory,
		HS_Shop_General
	};

	//Checks the HUD state then calls applyHUD
	void ApplyHUDChanges();

	//Getter HUDState
	uint8 GetHUDState();

	//Setter HUDState
	UFUNCTION(BlueprintCallable, Category = "HUD Functions")
	void ChangeHUDState(uint8 NewState);

	//Apply HUD to the screen
	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents);

protected:

	uint8 HUDState;
	//The HUD to be shown ingame
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> IngameHUDClass;
	//HUD to be shown in inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> InventoryHUDClass;
	//HUD to be shown in shop
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> ShopGeneralHUDClass;

	UPROPERTY()
	class UUserWidget* CurrentWidget;
};



