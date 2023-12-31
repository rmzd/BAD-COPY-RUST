// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandItems.h"
#include "Gun.generated.h"

UCLASS()
class MYPROJECT_API AGun : public AHandItems
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();
	FVector SpawnMuzzleFlashLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BulletSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	TArray<float> TimeOfShot;

	virtual void Tick(float DeltaTime) override;

	int MaxBulletInGun;

	int CurrentBulletInGun;

	int NumberOfShot = 0;

	int StageOfDirty;

	float TimeBtwnShots;

	float CoefficientOfRecoil;

	float TimeReload;

	float TimeClean;

	float TimeRepair;

	float Damage;

	float SprayEditHorizontal;

	float SprayEditVertical;

	float BestRangeBullet;

	float GoodRangeBullet;

	float BadRangeBullet;

	float SpeedBullet;

	float BestAccuracy;

	float Accuracy;

	float OverheatTime;

	bool OverheateWeapon;

	bool IsReloading;

	bool IsDirty;

	bool IsWatering;

	bool IsEquip;

	bool IsAuto;

	bool IsRepairing;

	bool IsCleaning;

	

	FTimerHandle TimeMakeDirty;

	FTimerHandle TimeWeaponOverheat;

	FTimerHandle TimeBetweenShots;

	float GetCoefficientOfRecoil();

	float GetSprayEditHorizontal();

	float GetSprayEditVertical();

	float GetAccuracy();

	float GetBestRangeBullet();

	float GetGoodRangeBullet();

	float GetBadRangeBullet();

	float GetDamage();

	float GetOverheatTime();

	void StartSymulatePhysics();

	void StopSymulatePhysics();

	void StopWeaponOverheated();

	void WeaponOverheated();

	void CleanWeapon();

	void unequip();

	void equip();

	void fire();

	void makeMoreDirty();

	void ReduceGunCharacteristic(int StagesOfDirty);

	bool canFire();

	void reload();

	void reloaded();

	void stopOverlap();

	void startOverlap();

	float GetTimeBtwShots();

	void RepairGun();

	void StopRepairing();

	void StopCleaning();
};