// Fill out your copyright notice in the Description page of Project Settings.


#include "Thompson.h"

AThompson::AThompson()
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

	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/pngwing_com__27_.pngwing_com__27_'")));
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/ Script / Engine.StaticMesh'/Game/thompson-m1928/source/Thompson.Thompson'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

	}
	VectorOfEquipPosition = FVector(10.f, 8.f,2);

	RotatorOfEquipPosition = FRotator(0.f, 70.f, 0);
}