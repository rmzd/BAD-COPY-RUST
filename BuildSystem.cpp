// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "wall.h"
#include "Engine/StaticMeshActor.h"
#include "floor.h"
#include "DoorFrame.h"
#include "Landscape.h"
#include "MyProjectProjectile.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"

ABuildSystem::ABuildSystem()
{
	PrimaryActorTick.bCanEverTick = true;


	GC = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Wood/GC_Stylized_Wood_Ceiling.GC_Stylized_Wood_Ceiling'")));
	GC_Stone = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Stone/GC_Stylized_Stone_Ceiling.GC_Stylized_Stone_Ceiling'")));
	GC_Metal = Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/EasyBuildingSystem/Chaos/Structures/Stylized/Metal/GC_Stylized_Metal_Ceiling.GC_Stylized_Metal_Ceiling'")));
	
	MeshOfWood = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Ceiling.SM_Stylized_Wood_Ceiling'")));

	
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GC"));
	if (GC != nullptr) {
		GeometryCollectionComponent->SetRestCollection(GC);
		GeometryCollectionComponent->SetSimulatePhysics(false);
		GeometryCollectionComponent->SetHiddenInGame(true);
	}


	Cylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder"));
	if (MeshOfWood != nullptr) {
		Cylinder->SetStaticMesh(MeshOfWood);
		Cylinder->SetupAttachment(GeometryCollectionComponent);
		Cylinder->SetWorldScale3D(FVector(1.f));
		Cylinder->SetWorldScale3D(FVector(1.f));
	}

	MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MyBoxComponent"));

	MyBoxComponent->InitBoxExtent(FVector(5.f));

	RootComponent = GeometryCollectionComponent;

	
	//	GeometryCollectionComponent->SetRestCollection(GC);
	MyBoxComponent->SetupAttachment(Cylinder);

	
	MyBoxComponent->SetCollisionProfileName("Trigger");

	MeshOfGreen = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Ceiling1.SM_Stylized_Wood_Ceiling1'")));

	MeshOfRed = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Ceiling2.SM_Stylized_Wood_Ceiling2'")));

	RedMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Script/Engine.MaterialInstanceConstant'/Engine/TemplateResources/RedMaterial.RedMaterial'")));

	StoneTextute = CreateDefaultSubobject<UMaterialInstance>(TEXT("OnMaterial"));
	MetalTextute = CreateDefaultSubobject<UMaterialInstance>(TEXT("OffMaterial"));

	StoneMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("StoneMeshzzz"));
	MetalMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("MetalMeshzzz"));

	leg_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("leg_1"));
	leg_2 = CreateDefaultSubobject<UBoxComponent>(TEXT("leg_2"));
	leg_3 = CreateDefaultSubobject<UBoxComponent>(TEXT("leg_3"));
	leg_4 = CreateDefaultSubobject<UBoxComponent>(TEXT("leg_4"));


	//было trigger
	leg_1->InitBoxExtent(FVector(5.f));
	leg_1->SetCollisionProfileName("Trigger");
	leg_1->SetRelativeLocation(FVector(145.f, -145.f, -10.f));

	leg_2->InitBoxExtent(FVector(5.f));
	leg_2->SetCollisionProfileName("Trigger");
	leg_2->SetRelativeLocation(FVector(145.f, 145.f, -10.f));

	leg_3->InitBoxExtent(FVector(5.f));
	leg_3->SetCollisionProfileName("Trigger");
	leg_3->SetRelativeLocation(FVector(-145.f, 145.f, -10.f));

	leg_4->InitBoxExtent(FVector(5.f));
	leg_4->SetRelativeLocation(FVector(-145.f, -145.f, -10.f));
	leg_4->SetCollisionProfileName("Trigger");
	MyBoxComponent->SetWorldScale3D(FVector(30.f, 30.f, 1.f));
	DoorHelper = false;

	NameOverlapObject = "Floor_12";

	IsOverlapping = false;


	MyBoxComponent->SetupAttachment(GeometryCollectionComponent);
	leg_1->SetupAttachment(Cylinder);
	leg_2->SetupAttachment(Cylinder);
	leg_3->SetupAttachment(Cylinder);
	leg_4->SetupAttachment(Cylinder);
	leg_1Over = leg_2Over = leg_3Over = leg_4Over = false;
	 
	MeshOfStone = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Polygonal/Stone/SM_Polygonal_Stone_Ceiling.SM_Polygonal_Stone_Ceiling'")));
	MeshOfMetal = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/EasyBuildingSystem/Meshes/Structures/Stylized/Metal/SM_Stylized_Metal_Ceiling.SM_Stylized_Metal_Ceiling'")));

	ShouldBeGreen = 1;

	StoneSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, TEXT("/Script/Engine.SoundWave'/Game/Sound/Build_stone.Build_stone'")));

	MetalSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, TEXT("/Script/Engine.SoundWave'/Game/Sound/Build_metal.Build_metal'")));
}

// Called when the game starts or when spawned
void ABuildSystem::BeginPlay()
{

	Super::BeginPlay();
	MyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABuildSystem::OnOverlapBegin);
	MyBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABuildSystem::OnOverlapEnd);

	 MyBoxComponent->OnComponentHit.AddDynamic(this, &ABuildSystem::OnHit);	// set up a notification for when this component hits something blocking

	leg_1->OnComponentBeginOverlap.AddDynamic(this, &ABuildSystem::OnOverlapBegin1);
	leg_2->OnComponentBeginOverlap.AddDynamic(this, &ABuildSystem::OnOverlapBegin2);
	leg_3->OnComponentBeginOverlap.AddDynamic(this, &ABuildSystem::OnOverlapBegin3);
	leg_4->OnComponentBeginOverlap.AddDynamic(this, &ABuildSystem::OnOverlapBegin4);


	leg_1->OnComponentEndOverlap.AddDynamic(this, &ABuildSystem::OnOverlapEnd1);
	leg_2->OnComponentEndOverlap.AddDynamic(this, &ABuildSystem::OnOverlapEnd2);
	leg_3->OnComponentEndOverlap.AddDynamic(this, &ABuildSystem::OnOverlapEnd3);
	leg_4->OnComponentEndOverlap.AddDynamic(this, &ABuildSystem::OnOverlapEnd4);

	Health = 1000;

}

// Called every frame
void ABuildSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildSystem::Destr()
{
	Destroy();

}



bool ABuildSystem::CanSpawn()
{
	return ((OverlapActor.Num() == 0) && (leg_1Over && leg_2Over && leg_3Over && leg_4Over));
}







void ABuildSystem::Damage(int HelthDamage)
{
	Health -= HelthDamage;

	if (Health <= 0) {
		if (Cylinder != nullptr) {
			Cylinder->DestroyComponent();

		}
		if (MyBoxComponent != nullptr) {

			MyBoxComponent->DestroyComponent();
		}

		if (GeometryCollectionComponent != nullptr) {
			GeometryCollectionComponent->SetHiddenInGame(false);
			GeometryCollectionComponent->SetSimulatePhysics(true);
		}
		

		
	}
}

void ABuildSystem::StopOverlap()
{

}

void ABuildSystem::Upgrate()
{

	if (levelOfMaterial == 1) {
		Health *= 2;

		if (MeshOfStone != nullptr) {
			Cylinder->SetStaticMesh(MeshOfStone);
		}
		levelOfMaterial++;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StoneSound"));

		if (StoneSound != nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StoneSound"));

			UGameplayStatics::PlaySoundAtLocation(this, StoneSound, GetActorLocation());
		}

	}

	else if(levelOfMaterial==2) {
		Health *= 2;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MetalSound"));

		if (MeshOfMetal != nullptr) {
			Cylinder->SetStaticMesh(MeshOfMetal);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Health %i"), Health));
		levelOfMaterial++;

		if (MetalSound != nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MetalSound"));
			UGameplayStatics::PlaySoundAtLocation(this, MetalSound, GetActorLocation());
		}
	}
}

void ABuildSystem::SetGreenMesh()
{
	
		if (OverlapActor.Num() == 0 && leg_1Over && leg_2Over && leg_3Over && leg_4Over) {

			Cylinder->SetStaticMesh(MeshOfGreen);
		}
		else {
			Cylinder->SetStaticMesh(MeshOfRed);

		}
	
	
	
}

void ABuildSystem::SetRedMesh()
{
	
	Cylinder->SetStaticMesh(MeshOfRed);

}



void ABuildSystem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {	
		if (DoorHelper) {
			if (!Cast<ADoorFrame>(OtherActor)) {
				if (!OverlapActor.Find(OtherActor)) {
					OverlapActor.Add(OtherActor);
					
				}
				
			}
		}
		else {
			OverlapActor.Add(OtherActor);
			if (!OverlapActor.Find(OtherActor)) {
				OverlapActor.Add(OtherActor);

			}
		}

	}
}






void ABuildSystem::OnOverlapBegin1(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (OtherActor->GetName()=="Landscape_0" || Cast<Afloor>(OtherActor) || Cast<Awall>(OtherActor)) {
			leg_1Over = true;

		}
	}
}

void ABuildSystem::OnOverlapBegin2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (OtherActor->GetName() == "Landscape_0" || Cast<Afloor>(OtherActor) || Cast<Awall>(OtherActor)) {
			leg_2Over = true;

		}
	}
}

void ABuildSystem::OnOverlapBegin3(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (OtherActor->GetName() == "Landscape_0" || Cast<Afloor>(OtherActor) || Cast<Awall>(OtherActor)) {
			leg_3Over = true;

		}
	}
}

void ABuildSystem::OnOverlapBegin4(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (OtherActor->GetName() == "Landscape_0" || Cast<Afloor>(OtherActor) || Cast<Awall>(OtherActor)) {
			leg_4Over = true;
		}
	}
}

void ABuildSystem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {

	}



}

void ABuildSystem::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnHit"));
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (Cast<AMyProjectProjectile>(OtherActor)) {
			Damage(50);
		}




	}
}






void ABuildSystem::OnOverlapEnd1(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (Cast<AStaticMeshActor>(OtherActor) || Cast<Afloor>(OtherActor) || Cast<Awall>(OtherActor)) {
			leg_1Over = false;
		}
	}
}

void ABuildSystem::OnOverlapEnd2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (Cast<AStaticMeshActor>(OtherActor) || Cast<Afloor>(OtherActor) || Cast<Awall>(OtherActor)) {
			leg_2Over = false;
		}
	}
}

void ABuildSystem::OnOverlapEnd3(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (Cast<AStaticMeshActor>(OtherActor) || Cast<Afloor>(OtherActor) || Cast<Awall>(OtherActor)) {
			leg_3Over = false;
		}
	}
}

void ABuildSystem::OnOverlapEnd4(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (Cast<AStaticMeshActor>(OtherActor) || Cast<Afloor>(OtherActor) || Cast<Awall>(OtherActor)) {
			leg_4Over = false;
		}
	}
}