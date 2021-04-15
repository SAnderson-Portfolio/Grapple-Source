// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Statue.h"

APickup_Statue::APickup_Statue()
{
}
void APickup_Statue::Use_Implementation(FVector location)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Statue!"));
	InteractableMesh->SetWorldLocation(location);
	InteractableMesh->SetVisibility(true);
	InteractableMesh->SetSimulatePhysics(true);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorLocation(location);
	
}
