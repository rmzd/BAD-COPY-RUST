// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class MYPROJECT_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UGeometryCollectionComponent* GeometryCollectionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* Cylinder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* MyBoxComponent;

	class UStaticMesh* StoneFloor;


	class UGeometryCollection* GC;

	class UStaticMesh* MeshOfWood;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void bbbb();
};
