// Fill out your copyright notice in the Description page of Project Settings.


#include "MetalAxeInventoryItems.h"
#include "Components/BoxComponent.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectCharacter.h"
#include "RawWolfMeat.h"
#include "AnimalBones.h"
#include "WolfSkin.h"

AMetalAxeInventoryItems::AMetalAxeInventoryItems() {
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Game/viking-axe/source/tools_3_low/tools_3_low.tools_3_low'")));
	if (StaticMesh != nullptr) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
	}
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Axe_good.Axe_good'")));

	VectorOfEquipPosition = FVector(-13.f, 3.f, 0);
	RotatorOfEquipPosition = FRotator(0, 90.f, -25.f);
	BlockCapsule->InitBoxExtent(FVector(100, 100.f, 100.f));

}
void AHandItems::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//ine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OnHit"));

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OnHit1"));

	}
}

void AHandItems::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (Cast<AAICharacter>(OtherActor)) {
			if (!helpvar) {
				class AActor* ActorToCraft;
				helpvar = 1;
				GetWorld()->GetTimerManager().SetTimer(timerelpvarreset, this, &AHandItems::helpvarreset, 1.5f, false);
				if (AxeSound != nullptr) {
					UGameplayStatics::PlaySoundAtLocation(this, AxeSound, GetActorLocation());
				}

				if (Cast<AAICharacter>(OtherActor)->GetMiningStage() == 0) {
					ActorToCraft = GetWorld()->SpawnActor<ARawWolfMeat>(FVector(0.f), FRotator(0.f));

					Cast<AMyProjectCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AddToInventory(ActorToCraft, 1);
					Cast<AAICharacter>(OtherActor)->IncrementMiningStage();
				}
				else if (Cast<AAICharacter>(OtherActor)->GetMiningStage() == 1) {
					ActorToCraft = GetWorld()->SpawnActor<ARawWolfMeat>(FVector(0.f), FRotator(0.f));

					Cast<AMyProjectCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AddToInventory(ActorToCraft, 2);

					ActorToCraft = GetWorld()->SpawnActor<AAnimalBones>(FVector(0.f), FRotator(0.f));

					Cast<AMyProjectCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AddToInventory(ActorToCraft, 2);
					Cast<AAICharacter>(OtherActor)->IncrementMiningStage();

				}
				else if (Cast<AAICharacter>(OtherActor)->GetMiningStage() == 2) {
					ActorToCraft = GetWorld()->SpawnActor<ARawWolfMeat>(FVector(0.f), FRotator(0.f));

					Cast<AMyProjectCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AddToInventory(ActorToCraft, 1);

					ActorToCraft = GetWorld()->SpawnActor<AAnimalBones>(FVector(0.f), FRotator(0.f));

					Cast<AMyProjectCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AddToInventory(ActorToCraft, 1);

					ActorToCraft = GetWorld()->SpawnActor<AWolfSkin>(FVector(0.f), FRotator(0.f));

					Cast<AMyProjectCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AddToInventory(ActorToCraft, 1);
					Cast<AAICharacter>(OtherActor)->IncrementMiningStage();

				}
			}
		}

	}
}

void AHandItems::helpvarreset()
{
	helpvar = 0;
}
