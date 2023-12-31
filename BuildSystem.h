// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildSystem.generated.h"

UCLASS()
class MYPROJECT_API ABuildSystem : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABuildSystem();

	class USoundBase* StoneSound;

	class USoundBase* MetalSound;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* Cylinder;

	class UGeometryCollection* GC;

	class UGeometryCollection* GC_Stone;

	class UGeometryCollection* GC_Metal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* MyBoxComponent;

	class UStaticMesh* StoneFloor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UGeometryCollectionComponent* GeometryCollectionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* leg_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* leg_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* leg_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* leg_4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInstance* StoneTextute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInstance* MetalTextute;

	class UMaterial* RedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* StoneMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* MetalMesh;

	class UStaticMesh* MeshOfWood;

	class UStaticMesh* MeshOfStone;

	class UStaticMesh* MeshOfMetal;

	class UStaticMesh* MeshOfGreen;

	class UStaticMesh* MeshOfRed;

	TArray <AActor*> OverlapActor;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	int ItemIndex;

	virtual void Tick(float DeltaTime) override;

	FString NameOverlapObject;

	void Destr();

	bool CanSpawn();

	bool IsOverlapping;

	bool DoorHelper;

	int levelOfMaterial=1;

	int Health;

	void Damage(int HelthDamage);

	void StopOverlap();

	void Upgrate();

	void SetGreenMesh();

	void SetRedMesh();

	bool ShouldBeGreen;

	bool isGreen;

	bool leg_1Over;

	bool leg_2Over;

	bool leg_3Over;

	bool leg_4Over;

	UFUNCTION()
	void OnOverlapBegin1(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBegin2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBegin3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBegin4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd1(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnOverlapEnd2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnOverlapEnd3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnOverlapEnd4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
