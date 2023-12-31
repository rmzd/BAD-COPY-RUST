// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneItems.h"
#include "Components/BoxComponent.h"

AStoneItems::AStoneItems() {


	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/STF/Pack03-LandscapePro/Environment/Foliage/RocksMedium/SM_rock01_lod000.SM_rock01_lod000'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
		StaticMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	}
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Stone.Stone'")));


	BlockCapsule->InitBoxExtent(FVector(30.f, 30.f, 20.f));
	NumItemInOne = 100;

}