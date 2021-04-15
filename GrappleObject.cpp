// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleObject.h"
#include "Components/StaticMeshComponent.h"
#include "CableComponent.h"
// Sets default values
AGrappleObject::AGrappleObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsGrappled = false;
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMesh");
	ObjectMesh->SetSimulatePhysics(true);
	Grapple = CreateDefaultSubobject<UCableComponent>(TEXT("GrappleRope"));
	Grapple->SetVisibility(false);
	SetRootComponent(ObjectMesh);
	ObjectMovementSpeed = 240.0f;
}

bool AGrappleObject::GetIsGrappled()
{
	return IsGrappled;
}

void AGrappleObject::SetIsGrappled(bool NewState)
{
	IsGrappled = NewState;
	ObjectMesh->SetEnableGravity(!ObjectMesh->IsGravityEnabled());
}

void AGrappleObject::SetOtherObject(AGrappleObject* otherObject)
{
	OtherObject = otherObject;
}

void AGrappleObject::SetIsFirstObject(bool newState)
{
	IsFirstObject = newState;
}

bool AGrappleObject::GetIsFirstObject()
{
	return IsFirstObject;
}



// Called when the game starts or when spawned
void AGrappleObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrappleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsGrappled)
	{
		if (OtherObject)
		{
			if (FVector(GetActorLocation() - OtherObject->GetActorLocation()).Size() > 200)
			{
				if (!IsSolid)
				{
					
					FVector direction = GetActorLocation() - OtherObject->GetActorLocation();
					direction.Normalize();
					SetActorLocation(GetActorLocation() + (-direction * (ObjectMovementSpeed * DeltaTime)));
				}
				
				if (IsFirstObject)
				{
					Grapple->SetVisibility(true);
					Grapple->EndLocation = GetActorForwardVector();
					Grapple->SetWorldLocation(FMath::VInterpTo(Grapple->K2_GetComponentLocation(), OtherObject->GetActorLocation(), GetWorld()->GetDeltaSeconds(), ObjectMovementSpeed));
				}
				
			}
			/*else
			{
				IsGrappled = false;
				OtherObject = nullptr;
				Grapple->SetVisibility(false);
				Grapple->EndLocation = FVector(0, 0, 0);
			}*/
		}
		
		
	}
	else
	{
		IsGrappled = false;
		OtherObject = nullptr;
		Grapple->SetVisibility(false);
		Grapple->EndLocation = FVector(0, 0, 0);
		ObjectMesh->SetEnableGravity(true);
	}

}

