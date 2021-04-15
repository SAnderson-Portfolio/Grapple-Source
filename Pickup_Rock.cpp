// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Rock.h"

APickup_Rock::APickup_Rock()
{

}

void APickup_Rock::Use_Implementation(FVector location)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Rock!"));
	InteractableMesh->SetWorldLocation(location);
	InteractableMesh->SetVisibility(true);
	InteractableMesh->SetSimulatePhysics(true);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}
