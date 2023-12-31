// Fill out your copyright notice in the Description page of Project Settings.
#include "Ak47.h"
#include "Components/BoxComponent.h"

AAk47::AAk47()
{
	MaxBulletInGun = 30;

	TimeBtwnShots = 0.1f;

	TimeReload = 3.f;

	Damage = 48.f;

	BestRangeBullet = 1000.f;

	GoodRangeBullet = 1700.f;

	BadRangeBullet = 2300.f;

	SpeedBullet = 22000.f;

	Accuracy = 1.7f;

	OverheatTime = 15.f;

	TimeClean = 4.f;

	TimeRepair = 4.f;

	CoefficientOfRecoil = 10.f;

	SprayEditHorizontal = 0.2f;

	SprayEditVertical = 4.f;

	IsAuto = 1;

	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/AK47.AK47'")));
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/ak-74/source/Ak_74_Low_Poly.Ak_74_Low_Poly'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	}
	VectorOfEquipPosition = FVector(9.f, 30.f, 6);

	RotatorOfEquipPosition = FRotator(-10.f, 80.f, 0);
	SpawnMuzzleFlashLocation = FVector(182, 58, -10.f);
	BulletSpawn->SetRelativeLocation(FVector(78, -2, -3));

}



void AAk47::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAk47::BeginPlay()
{
	Super::BeginPlay();
}

