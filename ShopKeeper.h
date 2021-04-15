// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShopKeeper.generated.h"

UCLASS()
class INVENTORY_API AShopKeeper : public AInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShopKeeper();

	virtual void BeginPlay() override;
	void Interact_Implementation() override;

	//The shop's inventory, represented by a Tarray of pickup objects
	UPROPERTY(EditAnywhere)
		TArray<APickUp*> Inventory;

	//Adds an item to the inventory
	UFUNCTION(BlueprintPure, Category = "Shop Functions")
		bool AddItemToShopInventory(APickUp* Item);

	//Uses the item at a given inventory slot
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
		void UseItemAtShopSlot(int32 Slot);

	//Gets the thumbnail for a given inventory slot
	UFUNCTION(BlueprintPure, Category = "Shop Functions")
		UTexture2D* GetThumbnailAtInventorySlot(int32 Slot);

	//Gets the item name for a given inventory slot
	UFUNCTION(BlueprintPure, Category = "Shop Functions")
		FString GetItemNameAtInventorySlot(int32 Slot);

	//Gets the item value for a given inventory slot
	UFUNCTION(BlueprintPure, Category = "Shop Functions")
		int32 GetItemValueAtInventorySlot(int32 Slot);

	UPROPERTY(EditAnywhere)
		class APickUp* pickupOne;
};
