// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorFrame.h"
#include "Components/StaticMeshComponent.h" 
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"
ADoorFrame::ADoorFrame() {

	MyBoxComponent->SetRelativeRotation(FRotator(90.f, 90.f, 0.f));
	leg_1->SetRelativeLocation(FVector(200.f, 0.f, -150.f));
	leg_2->SetRelativeLocation(FVector(200.f, 0.f, -150.f));
	leg_3->SetRelativeLocation(FVector(-200.f, 0.f, -150.f));
	leg_4->SetRelativeLocation(FVector(-200.f, 0.f, -150.f));
	MyBoxComponent->SetWorldScale3D(FVector(25.f, 25.f, 1.f));

	MeshOfWood = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Doorframe.SM_Stylized_Wood_Doorframe'")));

	GC = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Wood/GC_Stylized_Wood_DoorFrame.GC_Stylized_Wood_DoorFrame'")));
	GC_Stone = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Stone/GC_Stylized_Stone_DoorFrame.GC_Stylized_Stone_DoorFrame'")));
	GC_Metal = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Metal/GC_Stylized_Metal_DoorFrame.GC_Stylized_Metal_DoorFrame'")));


	if (GC != nullptr) {
		GeometryCollectionComponent->SetRestCollection(GC);

	}
	if (MeshOfWood != nullptr) {
		Cylinder->SetStaticMesh(MeshOfWood);

	}

	leg_1->SetWorldScale3D(FVector(10.f, 1.f, 1.f));

	leg_2->SetWorldScale3D(FVector(10.f, 1.f, 1.f));

	leg_3->SetWorldScale3D(FVector(10.f, 1.f, 1.f));

	leg_4->SetWorldScale3D(FVector(10.f, 1.f, 1.f));

	MeshOfStone = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Stone/SM_Stylized_Stone_Doorframe.SM_Stylized_Stone_Doorframe'")));
	 MeshOfMetal = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Metal/SM_Stylized_Metal_Doorframe.SM_Stylized_Metal_Doorframe'")));
	
	 MeshOfGreen = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Doorframe1.SM_Stylized_Wood_Doorframe1'")));

	 MeshOfRed = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Doorframe2.SM_Stylized_Wood_Doorframe2'")));


	ItemIndex = 3;
}

void ADoorFrame::BeginPlay() {
	Super::BeginPlay();

}