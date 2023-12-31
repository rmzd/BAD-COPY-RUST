// Fill out your copyright notice in the Description page of Project Settings.


#include "RevolverItems.h"
#include "Components/BoxComponent.h"

ARevolverItems::ARevolverItems() {
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/colt-peacemaker-revolver-1873/source/Model/Colt_Peacemaker_1873.Colt_Peacemaker_1873'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);

	}
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/pngwing_com__25_.pngwing_com__25_'")));

	BlockCapsule->InitBoxExtent(FVector(32.f, 2.f, 9.f));

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

	VectorOfEquipPosition = FVector(-14.f, -10.f, 0);

	RotatorOfEquipPosition = FRotator(0, -90.f, 30.f);
}