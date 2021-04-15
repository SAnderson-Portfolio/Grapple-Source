// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "InventoryProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GrappleTarget.h"
#include "InventoryGameMode.h"
#include "InventoryCharacter.h"
#include "PickUp.h"
#include "Kismet/GameplayStatics.h"
#include "CableComponent.h"
#include "GrappleObject.h"

AInventoryProjectile::AInventoryProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AInventoryProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	//ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;


	
}

void AInventoryProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (AGrappleTarget* Target = Cast<AGrappleTarget>(OtherActor))
	{
		if (AInventoryGameMode* GM = Cast<AInventoryGameMode>(GetWorld()->GetAuthGameMode()))
		{
			//GM->OnTargetHit();
			//ASpherePawn* player = Cast<ASpherePawn>(UGameplayStatics::GetPlayerPawn(this, 0));
			
		}
		AInventoryCharacter* player = Cast<AInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		player->SetAttachedLocation(GetActorLocation());
		Destroy();
	}

	if (APickUp* pickup = Cast<APickUp>(OtherActor))
	{
		AInventoryCharacter* player = Cast<AInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		pickup->Grapple_Interact_Implementation(player->GetActorLocation());
		Destroy();
	}

	if (AGrappleObject* Object = Cast<AGrappleObject>(OtherActor))
	{
		AInventoryCharacter* player = Cast<AInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		player->SetTargetObject(Object);
	}
	Destroy();
}