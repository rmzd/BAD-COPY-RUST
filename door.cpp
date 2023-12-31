// Fill out your copyright notice in the Description page of Project Settings.


#include "door.h"

#include "Components/StaticMeshComponent.h" 
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"
Adoor::Adoor() {
	MeshOfWood = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Door.SM_Stylized_Wood_Door'")));
	GC = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Wood/GC_Stylized_Wood_Door.GC_Stylized_Wood_Door'")));
	GC_Stone = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Stone/GC_Stylized_Stone_Door.GC_Stylized_Stone_Door'")));
	GC_Metal = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Metal/GC_Stylized_Metal_Door.GC_Stylized_Metal_Door'")));

	if (GC != nullptr) {
		GeometryCollectionComponent->SetRestCollection(GC);

	}
	if (MeshOfWood != nullptr) {
		Cylinder->SetStaticMesh(MeshOfWood);

	}
	
	GeometryCollectionComponent->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	Cylinder->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
	MyBoxComponent->SetRelativeRotation(FRotator(90.f, 90.f, 0.f));
	MyBoxComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
	leg_1->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	leg_2->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	leg_3->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	leg_4->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	leg_1Over = leg_2Over = leg_3Over = leg_4Over = true;
	MyBoxComponent->SetWorldScale3D(FVector(16.f, 10.f, 1.f));




	MeshOfStone = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Stone/SM_Stylized_Stone_Door.SM_Stylized_Stone_Door'")));
	MeshOfMetal = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Metal/SM_Stylized_Metal_Door.SM_Stylized_Metal_Door'")));

	MeshOfGreen = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Door1.SM_Stylized_Wood_Door1'")));

	MeshOfRed = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Door2.SM_Stylized_Wood_Door2'")));


	OpenDoor = false;

	DoorHelper = true;
	doorClose = FRotator(0.f, 0.f, 0.f);
	ItemIndex = 4;
}

void Adoor::OpenedDoor()
{
	if (this->GetActorRotation() == doorClose) {
		this->SetActorRotation(FRotator(this->GetActorRotation() + FRotator(0.f, -90.f, 0.f)));
		this->SetActorLocation(this->GetActorLocation() + FVector(-60.f, -60.f, 0.f));
		doorOpen = this->GetActorRotation();
	}
	else {
		this->SetActorRotation(FRotator(this->GetActorRotation() + FRotator(0.f, 90.f, 0.f)));
		this->SetActorLocation(this->GetActorLocation() + FVector(-60.f, -60.f, 0.f));
	}



	OpenDoor = true;
}

void Adoor::CloseDoor()
{
	if (this->GetActorRotation() == doorOpen) {
		this->SetActorRotation(FRotator(this->GetActorRotation() + FRotator(0.f, 90.f, 0.f)));
		this->SetActorLocation(this->GetActorLocation() + FVector(60.f, 60.f, 0.f));
		doorClose = this->GetActorRotation();
	}
	else {

		this->SetActorRotation(FRotator(this->GetActorRotation() + FRotator(0.f, -90.f, 0.f)));
		this->SetActorLocation(this->GetActorLocation() + FVector(60.f, 60.f, 0.f));
	}
	OpenDoor = false;
}

bool Adoor::IsOpen()
{
	return OpenDoor;
}





void Adoor::BeginPlay() {
	Super::BeginPlay();

}