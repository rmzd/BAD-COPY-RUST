// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

void AAIEnemyController::BeginPlay()
{
    Super::BeginPlay();

    NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
}


void AAIEnemyController::RandomPatrol()
{
    if (NavArea!=nullptr)
    {
        NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(),
            RandomLocation, 15000.0f);
        MoveToLocation(RandomLocation);
    }
}

void AAIEnemyController::MoveTo(FVector WhereMove)
{
    MoveToLocation(WhereMove);
}
