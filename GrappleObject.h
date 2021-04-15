// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrappleObject.generated.h"

UCLASS()
class INVENTORY_API AGrappleObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappleObject();
	UFUNCTION()
		bool GetIsGrappled();
	UFUNCTION()
		void SetIsGrappled(bool NewState);
	UFUNCTION()
		void SetOtherObject(AGrappleObject* otherObject);
	UFUNCTION()
		void SetIsFirstObject(bool newState);
	UFUNCTION()
		bool GetIsFirstObject();
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* ObjectMesh;
	UPROPERTY(EditAnywhere)
		bool IsSolid;
	UPROPERTY(EditAnywhere)
		float ObjectMovementSpeed;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	bool IsGrappled;
	bool IsFirstObject;
	class AGrappleObject* OtherObject;
	class UCableComponent* Grapple;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
