// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Unreal.h"

APickup_Unreal::APickup_Unreal()
{

}

void APickup_Unreal::Use_Implementation(FVector location)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unreal!"));
	InteractableMesh->SetWorldLocation(location);
	InteractableMesh->SetVisibility(true);
	InteractableMesh->SetSimulatePhysics(true);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorLocation(location);
}
