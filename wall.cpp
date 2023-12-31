// Fill out your copyright notice in the Description page of Project Settings.


#include "wall.h"

#include "Components/StaticMeshComponent.h" 
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"

Awall::Awall() {
	GC = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Wood/GC_Stylized_Wood_Wall.GC_Stylized_Wood_Wall'")));
	GC_Stone = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Stone/GC_Stylized_Stone_Wall.GC_Stylized_Stone_Wall'")));
	GC_Metal = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Metal/GC_Stylized_Metal_Wall.GC_Stylized_Metal_Wall'")));

	MeshOfWood = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Wall.SM_Stylized_Wood_Wall'")));

	MeshOfStone = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Stone/SM_Stylized_Stone_Wall.SM_Stylized_Stone_Wall'")));
	MeshOfMetal = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Metal/SM_Stylized_Metal_Wall.SM_Stylized_Metal_Wall'")));

	MeshOfGreen = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Wall1.SM_Stylized_Wood_Wall1'")));

	MeshOfRed = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Wall2.SM_Stylized_Wood_Wall2'")));


	if (GC != nullptr) {
		GeometryCollectionComponent->SetRestCollection(GC);
		
	}
	if (MeshOfWood != nullptr) {
		Cylinder->SetStaticMesh(MeshOfWood);
		Cylinder->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	}


	leg_1->SetWorldScale3D(FVector(10.f, 1.f, 1.f));
	leg_2->SetWorldScale3D(FVector(10.f, 1.f, 1.f));
	leg_3->SetWorldScale3D(FVector(10.f, 1.f, 1.f));
	leg_4->SetWorldScale3D(FVector(10.f, 1.f, 1.f));

	GeometryCollectionComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	leg_1->SetRelativeLocation(FVector(-0, -15, -150.f));
	leg_2->SetRelativeLocation(FVector(-50, -15, -150.f));
	leg_3->SetRelativeLocation(FVector(50, -15, -150.f));
	leg_4->SetRelativeLocation(FVector(100, -15, -150.f));
	leg_1->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	leg_2->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	leg_3->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	leg_4->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));


	MyBoxComponent->SetWorldScale3D(FVector(25.f, 25.f, 1.f));
	MyBoxComponent->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));

	ItemIndex = 1;
}



void Awall::BeginPlay() {
	Super::BeginPlay();

}