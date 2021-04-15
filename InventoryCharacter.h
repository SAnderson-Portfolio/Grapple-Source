// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interactable.h"
#include "PickUp.h"
#include "InventoryCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

public:
	AInventoryCharacter();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AInventoryProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	//The player's help text
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString HelpText;

	//The amount of gold the player had
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int32 Gold;

	//Updates the coins @param Amount this is amount to update the coind by, and can be positiive or negative
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
		void UpdateGold(int32 Amount);

	//Adds an item to the inventory
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		bool AddItemToInventory(APickUp* Item);

	//Gets the thumbnail for a given inventory slot
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		UTexture2D* GetThumbnailAtInventorySlot(int32 Slot);

	//Gets the item name for a given inventory slot
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		FString GetItemNameAtInventorySlot(int32 Slot);
	//Gets the item value for a given inventory slot
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		int32 GetItemValueAtInventorySlot(int32 Slot);
	//Uses the item at a given inventory slot
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
		void UseItemAtInventorySlot(int32 Slot);

	//Uses the item at a given inventory slot
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
		void UseItemAtShopSlot(int32 Slot);
	//Sets the location the attached location
	//@Param location the bullet has hit.
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetAttachedLocation(FVector NewLocation);

	//Sets the Target Object that has been hit
	//@Param pointer to object that has been hit
	UFUNCTION(BlueprintCallable, Category = "Targets")
		void SetTargetObject(class AGrappleObject* newObject);

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

private:
	//The player's reach range
	float Reach;
	//The grapple's reach range
	float grappleRopeLength;
	//Toggles the inventory.
	void ToggleInventory();
	//Cancels the grapple if it is stuck
	void ToggleGrapple();
	//Interacts with the current interactable.
	void Interact();
	//Checks for Interactable items in front of the player using line trace
	void CheckForInteractables();
	//Checks for targets at the player's target
	void CheckForTargets();
	//The interactable the player is currently looking at
	AInteractable* CurrentInteractable;

	//The player's inventory, represented by a Tarray of pickup objects
	UPROPERTY(EditAnywhere)
		TArray<APickUp*> Inventory;
	//The location the grapple has attached to
		FVector AttachedLocation;
	//If the grapple is attached
		bool Attached;
	//If the move is finished
		bool MoveFinished;
	//Cable component used to represent grappling line
		class UCableComponent* Grapple;
	//Speed the player moves towards attached location
	float PlayerMovementSpeed;
	//Function to move player character towards the attached location
	void MoveTo(float DeltaTime);

	//If Target One has been hit
	bool TargetOneAttached;
	//If Target Two has been hit
	bool TargetTwoAttached;
	//Pointer to the first object
	class AGrappleObject* ObjectOne;
	//Point to the secong object
	class AGrappleObject* ObjectTwo;
public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	

};

