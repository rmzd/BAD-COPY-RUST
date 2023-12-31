// Fill out your copyright notice in the Description page of Project Settings.


#include "Sg553Items.h"
#include "Components/BoxComponent.h"


ASg553Items::ASg553Items() {
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/sg553-assault-rifle/source/StaticMesh.StaticMesh'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	}
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/pngwing_com__26_.pngwing_com__26_'")));

	BlockCapsule->InitBoxExtent(FVector(2.f, 32.f, 9.f));

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

	VectorOfEquipPosition = FVector(0.f, 50.f, 0);

	RotatorOfEquipPosition = FRotator(0.f, 180.f, 0);

	IsAuto = 1;

	BulletSpawn->SetRelativeLocation(FVector(0, -46.f, 10.f));

}