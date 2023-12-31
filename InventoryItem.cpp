// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "Components/CapsuleComponent.h"
// Sets default values
AInventoryItem::AInventoryItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlockCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	BlockCapsule->SetupAttachment(RootComponent);
	BlockCapsule->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = BlockCapsule;
	GunSkelet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunSkelet"));
	GunSkelet->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInventoryItem::BeginPlay()
{
	CanStack = true;

	Super::BeginPlay();
	
}

// Called every frame
void AInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AInventoryItem::RetCanStack()
{
	return CanStack;
}

