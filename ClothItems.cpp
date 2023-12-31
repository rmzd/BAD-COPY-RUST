// Fill out your copyright notice in the Description page of Project Settings.


#include "ClothItems.h"
#include "Components/BoxComponent.h"

AClothItems::AClothItems() {
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/dirty-rag/source/Rag/Rag.Rag'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
		BlockCapsule->InitBoxExtent(FVector(20.f,20.f,5.f));
	}
	ItemIcon= Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Cloth.Cloth'")));
	NumItemInOne = 100;
}