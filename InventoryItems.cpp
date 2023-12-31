// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItems.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AInventoryItems::AInventoryItems()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlockCapsule = CreateDefaultSubobject<UBoxComponent>(TEXT("Capsule"));
	BlockCapsule->SetupAttachment(RootComponent);
	BlockCapsule->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = BlockCapsule;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/MS_Stump/3D_Asset/02_Lichen_Stump_rcquq/rcquq_LOD0.rcquq_LOD0'")));
	AddToInventorySound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, TEXT("/Script/Engine.SoundWave'/Game/add-to-inventory.add-to-inventory'")));
	CanStack = true;
	MaxStack = 50;
	NumItemInOne = 1;
	//ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/1653604190_1-kartinkin-net-p-brevno-kartinki-1.1653604190_1-kartinkin-net-p-brevno-kartinki-1'")));
}

// Called when the game starts or when spawned
void AInventoryItems::BeginPlay()
{
	
	Super::BeginPlay();

}

// Called every frame
void AInventoryItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInventoryItems::StartSymulatePhysics()
{
	StaticMeshComponent->SetSimulatePhysics(true);
	BlockCapsule->SetSimulatePhysics(true);
}

void AInventoryItems::StopSymulatePhysics()
{
	StaticMeshComponent->SetSimulatePhysics(false);
	BlockCapsule->SetSimulatePhysics(false);
}


bool AInventoryItems::GetCanStack()
{
	return CanStack;
}

int AInventoryItems::GetMaxStack()
{
	return MaxStack;
}

void AInventoryItems::Destroed()
{
	Destroy();
}
