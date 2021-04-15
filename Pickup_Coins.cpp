 // Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup_Coins.h"
#include "InventoryCharacter.h"


APickup_Coins::APickup_Coins()
{

}


void APickup_Coins::Interact_Implementation()
{
	AInventoryCharacter* Character = Cast<AInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	Character->UpdateGold(AmountOfCoins);
	Destroy();
}
