// Fill out your copyright notice in the Description page of Project Settings.


#include "MalletItems.h"
#include "Components/BoxComponent.h"

AMalletItems::AMalletItems() {
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT(" /Script/Engine.StaticMesh'/Game/Mallet/mallet.mallet'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	}
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/MALLET.MALLET'")));
	VectorOfEquipPosition = FVector(-13.f, 3.f, 0);
	RotatorOfEquipPosition = FRotator(0, 90.f, -25.f);
	BlockCapsule->InitBoxExtent(FVector(100, 100.f, 100.f));

}