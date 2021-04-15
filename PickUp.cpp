// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "InventoryCharacter.h"


APickUp::APickUp()
{
	//Setup the mesh for the pickup and set the item name, help text and item value
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	InteractableMesh->SetSimulatePhysics(true);

	ItemName = FString("Enter an item name here...");

	InteractableHelpText = FString("Press E to pick item up");
	Value = 0;
}
void APickUp::BeginPlay()
{
	InteractableHelpText = FString::Printf(TEXT("%s: Press E to pick up"), *ItemName);
}
void APickUp::Interact_Implementation()
{
	//Get a reference to character
	AInventoryCharacter* Character = Cast<AInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Character->AddItemToInventory(this))
	{
		OnPickUp();
	}
	
}

void APickUp::Grapple_Interact_Implementation(FVector playerPosition)
{
	do
	{
		FVector direction = GetActorLocation() - playerPosition;
		direction.Normalize();
		SetActorLocation(GetActorLocation() + (-direction * (24.0f * GetWorld()->GetDeltaSeconds())));
	} while (FVector(GetActorLocation() - playerPosition).Size() > 40);
}

void APickUp::Use_Implementation()
{
	GLog->Log("Use Implementation called from base class");
}

void APickUp::Use_Implementation(FVector location)
{
	GLog->Log("Use Implementation called from base class");
}

void APickUp::OnPickUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

int32 APickUp::GetValue()
{
	return Value;
}
