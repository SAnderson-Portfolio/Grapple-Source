// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrappleTarget.generated.h"

UCLASS()
class INVENTORY_API AGrappleTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappleTarget();

	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* TargetMesh;
	UPROPERTY(EditAnywhere, Category = "Components")
		UBillboardComponent* targetIcon;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void setAimedAt(bool isAimedAt);

private:
	bool AimedAt;
	float LifeTime;

};
