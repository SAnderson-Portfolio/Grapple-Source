// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/StaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class INVENTORY_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/*UFUNCTION(BlueprintNativeEvent)
		void Interact();*/

	virtual void Interact_Implementation();
	virtual void Grapple_Interact_Implementation(FVector playerPosition);
	UPROPERTY(EditAnywhere, Category = "Interactable Properties")
		class UStaticMeshComponent* InteractableMesh;

	UPROPERTY(EditAnywhere, Category = "Interactable Properties")
		FString InteractableHelpText;
};
