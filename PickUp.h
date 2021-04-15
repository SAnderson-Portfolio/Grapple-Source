// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "Interactable.h"
#include "PickUp.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API APickUp : public AInteractable
{
	GENERATED_BODY()
	
public:
	APickUp();

	virtual void BeginPlay() override;
	void Interact_Implementation() override;
	void Grapple_Interact_Implementation(FVector playerPosition) override;
	/*UFUNCTION(BlueprintNativeEvent)
		void Use();*/
	virtual void Use_Implementation();
	virtual void Use_Implementation(FVector location);
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	UTexture2D* PickupThumbnail;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	FString ItemName;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	int32 Value;
	//Called when the item is picked up
	void OnPickUp();
	//Returns value of the item
	int32 GetValue();
};
