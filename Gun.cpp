

#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "InventoryItem.h"
// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BulletSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("BulletSpawn"));
	BulletSpawn->SetupAttachment(RootComponent);
	BulletSpawn->SetCollisionProfileName(TEXT("NoCollision"));
	BulletSpawn->SetBoxExtent(FVector(1));

	
	FireSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, TEXT("/Script/Engine.SoundWave'/Game/03125.03125'")));

	MaxStack = 1;
	OverheateWeapon = false;
	StageOfDirty = 0;

	//where 1.0f - is the best
	BestAccuracy = Accuracy;
	SpawnMuzzleFlashLocation = FVector(150.f, 57.f, -25.f);

	IsAuto = 0;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	CurrentBulletInGun = MaxBulletInGun;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::equip() {
	if (GetWorld()->GetTimerManager().IsTimerPaused(TimeMakeDirty)) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TimerPaused"));
		GetWorld()->GetTimerManager().UnPauseTimer(TimeMakeDirty);
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TimerNoPaused"));
		IsEquip = true;
		GetWorld()->GetTimerManager().SetTimer(TimeMakeDirty, this, &AGun::makeMoreDirty, 5.f, false);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("equip"));
	}
}

float AGun::GetCoefficientOfRecoil()
{
	return CoefficientOfRecoil;
}

float AGun::GetSprayEditHorizontal()
{
	return SprayEditHorizontal;
}

float AGun::GetSprayEditVertical()
{
	return SprayEditVertical;
}

float AGun::GetAccuracy()
{
	return Accuracy;
}

float AGun::GetBestRangeBullet()
{
	return BestRangeBullet;
}

float AGun::GetGoodRangeBullet()
{
	return GoodRangeBullet;
}

float AGun::GetBadRangeBullet()
{
	return BadRangeBullet;
}

float AGun::GetDamage()
{
	return Damage;
}

float AGun::GetOverheatTime()
{
	return OverheatTime;
}



void AGun::StartSymulatePhysics() {
	StaticMeshComponent->SetSimulatePhysics(true);
	BlockCapsule->SetSimulatePhysics(true);
}

void AGun::StopSymulatePhysics()
{
	StaticMeshComponent->SetSimulatePhysics(false);
	BlockCapsule->SetSimulatePhysics(false);
}

void AGun::reload()
{
	IsReloading = true;
	CurrentBulletInGun = MaxBulletInGun;
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("bulletInGun: %i"), CurrentBulletInGun));
	GetWorld()->GetTimerManager().SetTimer(TimeBetweenShots, this, &AGun::reloaded, TimeReload, false);

}

void AGun::reloaded()
{
	IsReloading = false;
}
void AGun::makeMoreDirty()
{

	GetWorld()->GetTimerManager().SetTimer(TimeMakeDirty, this, &AGun::makeMoreDirty, 5.f, false);
	if (StageOfDirty < 5)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("makeMoreDirty"));
		StageOfDirty++;
		ReduceGunCharacteristic(StageOfDirty);
	}


	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("StageOfDirty: %i"), StageOfDirty));
}

void AGun::ReduceGunCharacteristic(int StagesOfDirty)
{
	Damage -= (Damage / 50) * StagesOfDirty;
	SprayEditHorizontal += (SprayEditHorizontal / 50) * StagesOfDirty;
	SprayEditVertical -= (SprayEditVertical / 50) * StagesOfDirty;
	CoefficientOfRecoil += (CoefficientOfRecoil / 50) * StagesOfDirty;
}


void AGun::unequip()
{
	GetWorld()->GetTimerManager().PauseTimer(TimeMakeDirty);
	IsEquip = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("unequip"));

}

void AGun::StopWeaponOverheated()
{
	OverheateWeapon = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NoWeaponOverheated"));
}

void AGun::WeaponOverheated()
{
	Accuracy += 0.1f;
	OverheateWeapon = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("WeaponOverheated"));
	GetWorld()->GetTimerManager().SetTimer(TimeWeaponOverheat, this, &AGun::StopWeaponOverheated, 3.f, false);
}

void AGun::fire() {
	if (canFire()) {
		Accuracy += 0.03f;
		CurrentBulletInGun--;
		NumberOfShot++;
		TimeOfShot.Add(GetWorld()->GetTimeSeconds());

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("TimeOfShot: %f"), GetWorld()->GetTimeSeconds()));
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("bulletInGun: %i"), CurrentBulletInGun));

		if (TimeOfShot.Num() == MaxBulletInGun * 2) {
			if (TimeOfShot[MaxBulletInGun * 2 - 1] - TimeOfShot[0] < GetOverheatTime()) {
				WeaponOverheated();
			}
			TimeOfShot.RemoveAt(0);
		}

	}
}

bool AGun::canFire() {
	return CurrentBulletInGun > 0 && !OverheateWeapon && !IsReloading;
}


float AGun::GetTimeBtwShots() {
	return TimeBtwnShots;
}

void AGun::CleanWeapon()
{
	if (!IsCleaning && !IsRepairing) {
		GetWorld()->GetTimerManager().SetTimer(TimeBetweenShots, this, &AGun::StopCleaning, TimeClean, false);
		IsCleaning = true;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CleanWeapon"));
	}
}

void AGun::RepairGun()
{
	if (!IsCleaning && !IsRepairing) {
		Accuracy = BestAccuracy;
		IsRepairing = true;
		GetWorld()->GetTimerManager().SetTimer(TimeBetweenShots, this, &AGun::StopRepairing, TimeRepair, false);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RepairGun"));
	}
}



void AGun::StopRepairing()
{
	IsRepairing = false;
	Accuracy = BestAccuracy;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopRepairing"));
}

void AGun::StopCleaning()
{
	IsCleaning = false;
	StageOfDirty = 0;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopCleaning"));
}