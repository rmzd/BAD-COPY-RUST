// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem.h"
#include "door.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API Adoor : public ABuildSystem
{
	GENERATED_BODY()

protected:
	void BeginPlay();

public:
	Adoor();
	void OpenedDoor();
	void CloseDoor();
	bool IsOpen();

	bool OpenDoor;

	FRotator doorOpen;

	FRotator doorClose;
};
