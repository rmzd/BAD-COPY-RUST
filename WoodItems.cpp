// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodItems.h"

AWoodItems::AWoodItems() {
	if (StaticMesh != nullptr) {
		StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/MS_Stump/3D_Asset/02_Lichen_Stump_rcquq/rcquq_LOD0.rcquq_LOD0'")));
		StaticMeshComponent->SetStaticMesh(StaticMesh);
	}
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'")));

	NumItemInOne = 100;
}