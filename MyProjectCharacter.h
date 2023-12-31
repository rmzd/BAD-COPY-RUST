// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyProjectCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	AMyProjectCharacter();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	class UTexture2D* IconEquipItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	class UTexture2D* IconNoEquipItem;

	UPROPERTY(EditAnywhere, Category = "Firing")
	class UNiagaraSystem* FireEffectMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* IconWhatCraft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
	TArray< TSubclassOf<AInventoryItems> > ArrayOfClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float health;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasPistol;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasPistol(bool bNewHasPistol);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasMallet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Menu)
	bool IsMenuOpen;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasMallet(bool bNewHasMallet);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* TriggerCapsule;

	UFUNCTION(BlueprintCallable)
	void FastCraft(int whatCraft);

	UFUNCTION(BlueprintCallable)
	void MakeEquipGun(int NumberOfInventoryItem);

	UFUNCTION(BlueprintCallable)
	void FunctionForCraft(int HowMuchCraft);

	UFUNCTION(BlueprintCallable)
	AInventoryItems* DeleteFromInventory(int whatDelete);

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMyProjectProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* MeleeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* Get_Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* Get_Rifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FirePistolAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* ReloadPistol;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<FString> PickItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<FString> PickItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<int> PickItemNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<UTexture2D*> Iconezzz;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<UTexture2D*> IconForCraft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<FString> NameForCraft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<int> NumForCraft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<int> RequiredNumForCraft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<FString> CraftItem;

	FVector MuzzleOffset;
	FCollisionQueryParams CollisionParams;
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

private:
	TArray<AInventoryItems*> PickItem2;

	class AActor* ActorToCraft;

	class Afloor* FloorSpawn;

	class Afloor* FloorSpawnDestroy;

	class Awall* WallSpawn;

	class Awall* WallSpawnDestroy;

	class Aroof* RoofSpawn;

	class Aroof* RoofSpawnDestroy;

	class ADoorFrame* DoorFrameSpawn;

	class ADoorFrame* DoorFrameSpawnDestroy;

	class Adoor* DoorSpawn;

	class Adoor* DoorSpawnDestroy;


	AActor* traceHit;

	FVector WhereBuild;

	FTimerHandle timerToShot;

	FTimerHandle timerToPlayEquipSound;

	TArray<AActor*> overlapItem;

	FRotator Wall1;

	FRotator Wall2;

	FRotator Wall3;

	FRotator Wall4;

	int whatBuild;

	bool IsFiring;

	int NumberOfBullet;

	int ShotsInaRow = 0;

	float CoefficientOfRecoil;

	float DifferentX;

	float DifferentY;

	float DifferentZ;

	float SprayEditHorizontal;

	float SprayEditVertical;

	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	bool isBuilding;

	bool CanUpgrate;
protected:
	virtual void BeginPlay();


public:
	class USoundBase* ReloadPistolSound;

	class USoundBase* WoodSound;

	class USoundBase* EquipGunSound;

	class USoundBase* ShootWithoutBullet;

	UFUNCTION(BlueprintCallable)
	void GetDataForCraft(int section, int numItem);

	void PlaySoundEquip();

	void Increment();

	void OpenDoor();

	void SpawnFloor(FVector location, FRotator rotation, bool ShouldOverlap, bool ShouldDestroy, bool ShouldRespawn);

	void SpawnRoof(FVector location, FRotator rotation, bool ShouldOverlap, bool ShouldDestroy, bool ShouldRespawn);

	void SpawnWall(FVector location, FRotator rotation, bool ShouldOverlap, bool ShouldDestroy, bool ShouldRespawn);

	void SpawnDoorFrame(FVector location, FRotator rotation, bool ShouldOverlap, bool ShouldDestroy, bool ShouldRespawn);

	void SpawnDoor(FVector location, FRotator rotation, bool ShouldOverlap, bool ShouldDestroy, bool ShouldRespawn);

	void Build();

	void OnStartSystem();

	void OnEndSystem();

	void OnBuildSystem();

	void OnDestroy();

	void OnDestroyWall();

	void OnDestroyRoof();

	void OnDestroyFloor();

	void OnDestroyDoor();

	void OnDestroyDoorFrame();

	void AddToInventoryByCraft(FString Name, int HowMuchAdd, UTexture2D* Icon);

	void LMB_click();

	bool FindItem(TArray<FString> ObjectsCraft, TArray<int> NeedToCraft, int HowMuchCraftbool, bool GetCraftNum);

	void LMB_NotClick();

	void LMB();

	void PickUp();

	void PickUpGun(class AHandItems* ItemOfInventory);

	void AddToInventory(class AActor* InventoryActor, int HowMuchAdd);

	void PickUpGun(class AHandItems* Over, FName nameSocket);

	void Drop();

	void SetEquippedGun();

	void RepairGun();

	void CleanGun();

	void DropGun(class AHandItems* Over);

	void Reload();

	void Change();

	void GetArrElem();

	bool Craft(TArray<FString> ObjectsCraft, TArray<int> NeedToCraft, int HowMuchCraft, bool GetCraftNum);


	class AInventoryItems* DeletedActorFromInventory;

	class AActor* GunOverlap;

	class AHandItems* BtwGun;

	class AHandItems* EquipGun;

	class AHandItems* NoEquipGun;

	class AMyProjectProjectile* SpawnedProjectile;

	TArray<AHandItems*> OverlapGun;

	class ABuildSystem* UpgrateObject;
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	TArray<FHitResult> HitResults;

	FTimerHandle TimeBtwShots;

	class AHandItems* ItemInventory;
};

