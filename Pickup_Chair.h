// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "Pickup_Chair.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API APickup_Chair : public APickUp
{
	GENERATED_BODY()
public:
		APickup_Chair();
		virtual void Use_Implementation(FVector location) override;
};
