// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopKeeper.h"
#include "InventoryGameMode.h"
#include "PickUp.h"
// Sets default values
AShopKeeper::AShopKeeper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShopKeepMesh");
}

// Called when the game starts or when spawned
void AShopKeeper::BeginPlay()
{
	Super::BeginPlay();
	InteractableHelpText = FString::Printf(TEXT("Shop Keep Press E to shop"));
	Inventory.SetNum(4);
	if (pickupOne)
		AddItemToShopInventory(pickupOne);
	
}

void AShopKeeper::Interact_Implementation()
{
	AInventoryGameMode* GameMode = Cast<AInventoryGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode->GetHUDState() == GameMode->HS_Ingame)
	{
		GameMode->ChangeHUDState(GameMode->HS_Shop_General);
	}
	else
	{
		GameMode->ChangeHUDState(GameMode->HS_Ingame);
	}
}

bool AShopKeeper::AddItemToShopInventory(APickUp* Item)
{
	if (Item != NULL)
	{
		const int32 AvailableSlot = Inventory.Find(nullptr);//Find first slot with a nullptr in it

		if (AvailableSlot != INDEX_NONE)
		{
			Inventory[AvailableSlot] = Item;
			return true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You can't carry anymore items!"));
			return false;
		}
	}
	return false;
}

void AShopKeeper::UseItemAtShopSlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		Inventory[Slot] = NULL; // Delete the item from inventory once used.
	}
}

UTexture2D * AShopKeeper::GetThumbnailAtInventorySlot(int32 Slot)
{
	
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->PickupThumbnail;
		
	}
	else return nullptr;
}

FString AShopKeeper::GetItemNameAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->ItemName;
	}
	return FString("None");
}

int32 AShopKeeper::GetItemValueAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->GetValue();
	}
	else
		return (int32)0;
}


