// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItems.h"
#include "HandItems.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AHandItems : public AInventoryItems
{
	GENERATED_BODY()


public:
	AHandItems();

	USoundBase* AxeSound;

	FVector VectorOfEquipPosition;

	FRotator RotatorOfEquipPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	float WearRaiting;

	virtual void BeginPlay();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool helpvar;

	void helpvarreset();

	FTimerHandle timerelpvarreset;

};
