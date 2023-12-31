// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryItem.generated.h"

UCLASS()
class MYPROJECT_API AInventoryItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventoryItem();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* BlockCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* GunSkelet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* ItemIcon;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	bool CanStack;

	bool IsGun;

	bool RetCanStack();
	//picture

	//ismedicine

	//isfood mb


};
