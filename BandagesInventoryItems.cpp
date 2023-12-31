// Fill out your copyright notice in the Description page of Project Settings.


#include "BandagesInventoryItems.h"

ABandagesInventoryItems::ABandagesInventoryItems() {
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT(" /Script/Engine.StaticMesh'/Game/Mallet/mallet.mallet'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

	}
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/pngwing_com__19_.pngwing_com__19_'")));

}