// Fill out your copyright notice in the Description page of Project Settings.


#include "MetalItems.h"
#include "Components/BoxComponent.h"

AMetalItems::AMetalItems() {
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Metal/SM_Stylized_Metal_Fence.SM_Stylized_Metal_Fence'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

		StaticMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
	}
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'")));
	NumItemInOne = 100;
	BlockCapsule->InitBoxExtent(FVector(32.f, 2.f, 9.f));

}
