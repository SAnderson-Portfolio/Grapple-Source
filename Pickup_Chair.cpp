// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup_Chair.h"

APickup_Chair::APickup_Chair()
{

}

void APickup_Chair::Use_Implementation(FVector location)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Chair!"));
	InteractableMesh->SetWorldLocation(location);
	InteractableMesh->SetVisibility(true);
	InteractableMesh->SetSimulatePhysics(true);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorLocation(location);
}
