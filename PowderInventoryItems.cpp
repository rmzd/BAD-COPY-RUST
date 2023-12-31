// Fill out your copyright notice in the Description page of Project Settings.


#include "PowderInventoryItems.h"

APowderInventoryItems::APowderInventoryItems() {
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT(" /Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Metal/SM_Stylized_Metal_Foundation.SM_Stylized_Metal_Foundation'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	}
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Powder.Powder'")));
	NumItemInOne = 100;

}