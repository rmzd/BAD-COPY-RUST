// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Ak47.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API AAk47 : public AGun
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAk47();



	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};