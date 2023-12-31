// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GC = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Polygonal/Wood/GC_Polygonal_Wood_Foundation.GC_Polygonal_Wood_Foundation'")));
	MeshOfWood = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Foundation.SM_Stylized_Wood_Foundation'")));


	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GC"));	
	GeometryCollectionComponent->SetSimulatePhysics(false);
	GeometryCollectionComponent->SetRestCollection(GC);
	Cylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder"));
	Cylinder->SetStaticMesh(MeshOfWood);

	MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MyBoxComponent"));

	MyBoxComponent->InitBoxExtent(FVector(5.f));



	RootComponent=GeometryCollectionComponent;

	Cylinder->SetupAttachment(GeometryCollectionComponent);
	Cylinder->SetWorldScale3D(FVector(1.f));
//	GeometryCollectionComponent->SetRestCollection(GC);
	MyBoxComponent->SetupAttachment(Cylinder);

	Cylinder->SetWorldScale3D(FVector(1.f));
	MyBoxComponent->SetCollisionProfileName("Trigger");
	MyBoxComponent->SetWorldScale3D(FVector(29.f, 29.f, 8.f));


}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::bbbb()
{
	Cylinder->SetHiddenInGame(true);
	Cylinder->SetSimulatePhysics(false);
	MyBoxComponent->SetCollisionProfileName("NoCollision");
	GeometryCollectionComponent->SetSimulatePhysics(true);
}

