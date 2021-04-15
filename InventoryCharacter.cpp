// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "InventoryCharacter.h"
#include "InventoryProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "InventoryGameMode.h"
#include "CableComponent.h"
#include "GrappleTarget.h"
#include "GrappleObject.h"
#include "ShopKeeper.h"
#include "UObject/UObjectGlobals.h"
DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AInventoryCharacter

AInventoryCharacter::AInventoryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	Reach = 250.0f;
	grappleRopeLength = 10000.0f;
	Attached = false;
	AttachedLocation = FVector(0.f, 0.f, 0.f);
	PlayerMovementSpeed = 1000.0f;
	MoveFinished = true;

	Grapple = CreateDefaultSubobject<UCableComponent>(TEXT("GrappleRope"));
	Grapple->SetVisibility(false);
	//Grapple->CableLength = 0.f;
	//Grapple->CableWidth = 3.5f;
	//Grapple->NumSegments = 8.f;
	//Grapple->NumSides = 8.f;
	//Grapple->bAttachEnd = true;
	//Grapple->bAttachStart = true;
	//Grapple->SetAttachEndToComponent(FP_Gun);
	//Grapple->Set
	//Grapple->SetupAttachment(FP_Gun);

	TargetOneAttached = false;
	TargetTwoAttached = false;
}

void AInventoryCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	Inventory.SetNum(4);
	CurrentInteractable = nullptr;

	Mesh1P->SetHiddenInGame(false, true);
}

void AInventoryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();
	CheckForTargets();
	if (Attached)
	{
		if (!MoveFinished)
		{
			MoveTo(DeltaTime);
		}
	}

	if (TargetOneAttached && TargetTwoAttached)
	{
		
		if (!ObjectOne->GetIsGrappled() || !ObjectTwo->GetIsGrappled())
		{
			ObjectOne = nullptr;
			TargetOneAttached = false;
			ObjectTwo = nullptr;
			TargetTwoAttached = false;
		}
		
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AInventoryCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AInventoryCharacter::Interact);
	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AInventoryCharacter::ToggleInventory);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AInventoryCharacter::OnFire);
	//Bind Cancel grapple event
	PlayerInputComponent->BindAction("ToggleGrapple", IE_Pressed, this, &AInventoryCharacter::ToggleGrapple);
	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AInventoryCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AInventoryCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AInventoryCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AInventoryCharacter::LookUpAtRate);
}

void AInventoryCharacter::UpdateGold(int32 Amount)
{
	Gold += Amount;
}

bool AInventoryCharacter::AddItemToInventory(APickUp * Item)
{
	if (Item != NULL)
	{
		const int32 AvailableSlot = Inventory.Find(nullptr);//Find first slot with a nullptr in it

		if (AvailableSlot != INDEX_NONE)
		{
			Inventory[AvailableSlot] = Item;
			return true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You can't carry anymore items!"));
			return false;
		}
	}
	return false;
}

UTexture2D * AInventoryCharacter::GetThumbnailAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->PickupThumbnail;
	}
	else return nullptr;
}

FString AInventoryCharacter::GetItemNameAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->ItemName;
	}
	return FString("None");
}

int32 AInventoryCharacter::GetItemValueAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->GetValue();
	}
	else
		return (int32)0;
}

void AInventoryCharacter::UseItemAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		Inventory[Slot]->Use_Implementation(GetActorLocation() + (GetActorForwardVector() * 3));
		Inventory[Slot] = NULL; // Delete the item from inventory once used.
	}
}

void AInventoryCharacter::UseItemAtShopSlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		Inventory[Slot]->Use_Implementation();
		UpdateGold(Inventory[Slot]->GetValue());
		Inventory[Slot] = NULL; // Delete the item from inventory once used.
	}
}

void AInventoryCharacter::SetAttachedLocation(FVector NewLocation)
{
	if (MoveFinished && !Attached)
	{
		AttachedLocation = NewLocation;
		MoveFinished = false;
		Attached = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Location set!"));
	}
}

void AInventoryCharacter::SetTargetObject(class AGrappleObject* newObject)
{
	if (!TargetOneAttached && !newObject->GetIsGrappled())
	{
		ObjectOne = newObject;
		TargetOneAttached = true;
		ObjectOne->SetIsFirstObject(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Target One Set!"));
	}
	else if (TargetOneAttached && !TargetTwoAttached && !newObject->GetIsGrappled())
	{
		ObjectTwo = newObject;
		TargetTwoAttached = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Target Two Set!"));
	}

	if (ObjectOne && ObjectTwo && !ObjectOne->GetIsGrappled() && !ObjectTwo->GetIsGrappled())
	{
		ObjectOne->SetIsGrappled(true);
		ObjectTwo->SetIsGrappled(true);
		ObjectTwo->SetIsFirstObject(false);
		ObjectOne->SetOtherObject(ObjectTwo);
		ObjectTwo->SetOtherObject(ObjectOne);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Objects set!"));
	}
}

void AInventoryCharacter::OnFire()
{
	AInventoryGameMode* GameMode = Cast<AInventoryGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode->GetHUDState() == GameMode->HS_Ingame)
	{
		// try and fire a projectile
		if (ProjectileClass != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				// spawn the projectile at the muzzle
				World->SpawnActor<AInventoryProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}

		// try and play the sound if specified
		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
	
}



void AInventoryCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AInventoryCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AInventoryCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AInventoryCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AInventoryCharacter::ToggleInventory()
{
	//Check player's HUD state, if inventory is open then close it, otherwise open inventory
	AInventoryGameMode* GameMode = Cast<AInventoryGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode->GetHUDState() == GameMode->HS_Ingame)
	{
		GameMode->ChangeHUDState(GameMode->HS_Inventory);
	}
	else
	{
		GameMode->ChangeHUDState(GameMode->HS_Ingame);
	}
}

void AInventoryCharacter::ToggleGrapple()
{
	if (ObjectOne)
	{
		ObjectOne->SetIsGrappled(false);
		ObjectOne = nullptr;
		TargetOneAttached = false;
	}
	
	if (ObjectTwo)
	{
		ObjectTwo->SetIsGrappled(false);
		ObjectTwo = nullptr;
		TargetTwoAttached = false;
	}

	if (Attached)
	{
		Grapple->SetWorldLocation(FVector(0, 0, 0));
		Grapple->SetVisibility(false);
		MoveFinished = true;
		Attached = false;
	}
	
}

void AInventoryCharacter::Interact()
{
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->Interact_Implementation();
	}
	
	
}

void AInventoryCharacter::CheckForInteractables()
{
	//To linetrace, get start and end traces
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndTrace = (FirstPersonCameraComponent->GetForwardVector() * Reach) + StartTrace;

	//Declare hitresult to store ray cast
	FHitResult HitResult;

	//Init the query params - ignore actor
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	//Cast the line trace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CollisionParams);

	AInteractable* PotentialInteractable = Cast<AInteractable>(HitResult.GetActor());

	if (PotentialInteractable == NULL)
	{
		HelpText = FString("");
		CurrentInteractable = nullptr;
		return;
	}
	else
	{
		CurrentInteractable = PotentialInteractable;
		HelpText = PotentialInteractable->InteractableHelpText;
	}

}

void AInventoryCharacter::CheckForTargets()
{
	//To linetrace, get start and end traces
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndTrace = (FirstPersonCameraComponent->GetForwardVector() * grappleRopeLength) + StartTrace;

	//Declare hitresult to store ray cast
	FHitResult HitResult;

	//Init the query params - ignore actor
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	//Cast the line trace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CollisionParams);

	AGrappleTarget* Target = Cast<AGrappleTarget>(HitResult.GetActor());
	//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, 1, 0, 5);
	if (Target == NULL)
	{
		return;
	}
	else
	{
		Target->setAimedAt(true);
	}
}

void AInventoryCharacter::MoveTo(float DeltaTime)
{
	if (FVector(GetActorLocation() - AttachedLocation).Size() > 50)
	{
		Grapple->SetVisibility(true);
		Grapple->EndLocation = GetActorForwardVector();
		Grapple->SetWorldLocation(FMath::VInterpTo(Grapple->K2_GetComponentLocation(), AttachedLocation, GetWorld()->GetDeltaSeconds(), PlayerMovementSpeed));
		FVector direction = GetActorLocation() - AttachedLocation;
		direction.Normalize();
		LaunchCharacter(-direction * PlayerMovementSpeed, true, true);
	}
	else
	{
		Grapple->SetWorldLocation(FVector(0, 0, 0));
		Grapple->SetVisibility(false);
		MoveFinished = true;
		Attached = false;
	}
}
