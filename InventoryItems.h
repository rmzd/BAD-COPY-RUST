// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryItems.generated.h"

UCLASS()
class MYPROJECT_API AInventoryItems : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AInventoryItems();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BlockCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComponent;

	class UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* AddToInventorySound;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	bool CanStack;

	int MaxStack;

	int NumItemInOne;

	bool IsGun;

	void StartSymulatePhysics();
	
	void StopSymulatePhysics();

	bool GetCanStack();

	int GetMaxStack();

	void Destroed();
	//ismedicine

	//isfood mb


};
