// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "Pickup_Coins.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API APickup_Coins : public APickUp
{
	GENERATED_BODY()
public:
		APickup_Coins();
		virtual void Interact_Implementation() override;

	UPROPERTY(EditAnywhere, Category = "CoinsProperty")
	int32 AmountOfCoins;


};
