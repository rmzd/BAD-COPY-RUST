// Fill out your copyright notice in the Description page of Project Settings.


#include "floor.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"
Afloor::Afloor() {
	MyBoxComponent->SetWorldScale3D(FVector(29.f, 29.f, 6.f));
	MeshOfWood = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Foundation.SM_Stylized_Wood_Foundation'")));

	leg_1->SetRelativeLocation(FVector(115.f, -125.f, -45.f));
	leg_1->SetWorldScale3D(FVector(3.f, 3.f, 3.f));
	GC = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Wood/GC_Stylized_Wood_Foundation.GC_Stylized_Wood_Foundation'")));
	GC_Stone = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Stone/GC_Stylized_Stone_Foundation.GC_Stylized_Stone_Foundation'")));
	GC_Metal = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Metal/GC_Stylized_Metal_Foundation.GC_Stylized_Metal_Foundation'")));


	if (GC != nullptr) {
		GeometryCollectionComponent->SetRestCollection(GC);

	}
	if (MeshOfWood != nullptr) {
		Cylinder->SetStaticMesh(MeshOfWood);

	}
	leg_2->SetRelativeLocation(FVector(115.0, 125.f, -45.f));
	leg_2->SetWorldScale3D(FVector(3.f, 3.f, 3.f));

	leg_3->SetRelativeLocation(FVector(-115.f, 125.f, -45.f));
	leg_3->SetWorldScale3D(FVector(3.f, 3.f, 3.f));

	leg_4->SetRelativeLocation(FVector(-115.f, -125.f, -45.f));
	leg_4->SetWorldScale3D(FVector(3.f, 3.f, 3.f));



	MeshOfStone = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Stone/SM_Stylized_Stone_Foundaton.SM_Stylized_Stone_Foundaton'")));
	MeshOfMetal = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Metal/SM_Stylized_Metal_Foundation.SM_Stylized_Metal_Foundation'")));

	MeshOfGreen = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Foundation1.SM_Stylized_Wood_Foundation1'")));

	MeshOfRed = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Foundation2.SM_Stylized_Wood_Foundation2'")));
	ItemIndex = 0;

}

void Afloor::BeginPlay()
{
	Super::BeginPlay();
}

void Afloor::Tick(float DeltaTime)
{

}
