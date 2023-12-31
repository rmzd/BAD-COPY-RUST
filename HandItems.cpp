// Fill out your copyright notice in the Description page of Project Settings.


#include "HandItems.h"
#include "Components/BoxComponent.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectCharacter.h"
#include "RawWolfMeat.h"
#include "AnimalBones.h"
#include "WolfSkin.h"

AHandItems::AHandItems() {
	BlockCapsule->SetCollisionProfileName(TEXT("OverlapAll"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("Gun"));

	WearRaiting = 1; 

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AHandItems::OnHit);		// set up a notification for when this component hits something blocking
	AxeSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, TEXT("/Script/Engine.SoundWave'/Game/Sound/Axe_hit.Axe_hit'")));

}

void AHandItems::BeginPlay()
{	
	BlockCapsule->OnComponentBeginOverlap.AddDynamic(this, &AHandItems::OnOverlapBegin);

}


