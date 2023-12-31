// Fill out your copyright notice in the Description page of Project Settings.


#include "Deer.h"

ADeer::ADeer() {


	
}

void ADeer::BeginPlay()
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
	
	Super::BeginPlay();
}

void ADeer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}