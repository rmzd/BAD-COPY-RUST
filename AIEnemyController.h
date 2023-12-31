// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()

public:
    void BeginPlay() override;

    UFUNCTION()
    void RandomPatrol();

    void MoveTo(FVector WhereMove);
private:

    class UNavigationSystemV1* NavArea;

    FVector RandomLocation;

    FTimerHandle moveTimer;


};
