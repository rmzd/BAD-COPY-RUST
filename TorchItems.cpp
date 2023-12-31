// Fill out your copyright notice in the Description page of Project Settings.


#include "TorchItems.h"
ATorchItems::ATorchItems() {
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/texture/fakel.fakel'")));

}
