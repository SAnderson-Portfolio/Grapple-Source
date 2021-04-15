// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "Pickup_Unreal.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API APickup_Unreal : public APickUp
{
	GENERATED_BODY()
	
public:
	APickup_Unreal();
	virtual void Use_Implementation(FVector location) override;
};
