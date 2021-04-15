// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

// Sets default values
AInteractable::AInteractable()
{
	InteractableHelpText = FString("Press E to interact with item");
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractable::Interact_Implementation()
{
	GLog->Log("Interact base class: Interact()");
}

void AInteractable::Grapple_Interact_Implementation(FVector playerPosition)
{
	GLog->Log("Interact base class: Grapple_Interact()");
}
