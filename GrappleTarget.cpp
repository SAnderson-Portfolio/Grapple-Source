// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleTarget.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BillboardComponent.h"
// Sets default values
AGrappleTarget::AGrappleTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>("TargetMesh");
	TargetMesh->SupportsDefaultCollision();
	TargetMesh->UpdateCollisionFromStaticMesh();
	targetIcon = CreateDefaultSubobject<UBillboardComponent>("TargeIcon");
	targetIcon->SetVisibility(false);
	targetIcon->SetupAttachment(TargetMesh);
	//targetIcon->SetWorldLocation(TargetMesh->GetComponentLocation());
	targetIcon->SetRelativeLocation(FVector(TargetMesh->GetComponentLocation().X + 15, TargetMesh->GetComponentLocation().Y, TargetMesh->GetComponentLocation().Z + 8));
	targetIcon->bHiddenInGame = false;
	AimedAt = false;
	SetRootComponent(TargetMesh);
	LifeTime = 0.0f;
}

// Called when the game starts or when spawned
void AGrappleTarget::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AGrappleTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AimedAt)
	{
		targetIcon->SetVisibility(true);
		LifeTime -= 1.0f;
		if (LifeTime <= 0.0f)
			AimedAt = false;
	}
	else
	{
		targetIcon->SetVisibility(false);
	}
}

void AGrappleTarget::setAimedAt(bool isAimedAt)
{
	AimedAt = isAimedAt;
	LifeTime = 2.0f;
}

