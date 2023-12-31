// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneAxeInventoryItems.h"

AStoneAxeInventoryItems::AStoneAxeInventoryItems() {
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/stone-axe/source/stone_axe/stone_axe.stone_axe'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
	}
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/HATCHET.HATCHET'")));

}