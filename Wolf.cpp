// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf.h"


AWolf::AWolf() {



}

void AWolf::BeginPlay()
{
	Health = 100;

	MinSpeedRun = 700;

	MaxSpeedRun = 900;

	MaxHealth = 100;

	MaxFood = 100;

	MaxSleep = 100;

	IsEating = false;

	IsSleeping = false;

	IsHealthing = false;

	IsPredator = 1;

	Super::BeginPlay();
}

void AWolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}