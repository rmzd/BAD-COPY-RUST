// Fill out your copyright notice in the Description page of Project Settings.


#include "RawWolfMeat.h"

ARawWolfMeat::ARawWolfMeat() {
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT(" /Script/Engine.StaticMesh'/Game/Mallet/mallet.mallet'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	}
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Raw_Wolf_Meat_icon.Raw_Wolf_Meat_icon'")));

}