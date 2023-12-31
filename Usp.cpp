// Fill out your copyright notice in the Description page of Project Settings.


#include "Usp.h"

AUsp::AUsp()
{
	MaxBulletInGun = 12;

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

	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/pngwing_com__29_.pngwing_com__29_'")));
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/hk-usp/source/HK_USP.HK_USP'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(0.05, 0.05, 0.05));

	}
	VectorOfEquipPosition = FVector(-15.f, -10.f, 0);
	SpawnMuzzleFlashLocation = FVector(120.f, -57.f, -25.f);

	RotatorOfEquipPosition = FRotator(30, -180.f, 20.f);
}