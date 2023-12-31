// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "InventoryItem.h"
#include "MyProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AICharacter.h"
#include "Gun.h"
#include "InventoryItems.h"
#include "BuildSystem.h"
#include "wall.h"
#include "floor.h"
#include "roof.h"
#include "DoorFrame.h"
#include "door.h"
#include "MetalItems.h"
#include "TorchItems.h"
#include "MalletItems.h"
#include "WoodItems.h"
#include "ClothItems.h"
#include "StoneItems.h"
#include "MetalAxeInventoryItems.h"
#include "StoneAxeInventoryItems.h"
#include "BandagesInventoryItems.h"
#include "SpringInventoryItems.h"
#include "Ak47.h"
#include "HandItems.h"
#include "Sg553Items.h"
#include "RevolverItems.h"
#include "Makarov.h"
#include "Usp.h"
#include "Thompson.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyActor.h"
#include "Components/BoxComponent.h"

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	MuzzleOffset = FVector(100.0f, 60.0f, 20.0f);

	health = 100;

	ReloadPistolSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, TEXT("/Script/Engine.SoundWave'/Game/Sound/ReloadPistol.ReloadPistol'")));

	WoodSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, TEXT("/Script/Engine.SoundWave'/Game/Sound/Build_wood.Build_wood'")));

	EquipGunSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, TEXT("/Script/Engine.SoundWave'/Game/Sound/aug_boltslap.aug_boltslap'")));

	ShootWithoutBullet = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, TEXT("/Script/Engine.SoundWave'/Game/Sound/Shoot-without-bullet.Shoot-without-bullet'")));
}

void AMyProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}







void AMyProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector loc1;
	FRotator rot1;
	FHitResult Hit;
	GetController()->GetPlayerViewPoint(loc1, rot1);
	FVector start1 = loc1;
	FVector end1 = start1 + (rot1.Vector() * 700);
	// FHitResult will hold all data returned by our line collision query


	// We set up a line trace from our current location to a point 1000cm ahead of us

	// You can use FCollisionQueryParams to further configure the query
	// Here we add ourselves to the ignored list so we won't block the trace
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (isBuilding) {
		if (FloorSpawnDestroy != nullptr) {
			QueryParams.AddIgnoredActor(FloorSpawnDestroy); 
			
		}
		if (RoofSpawnDestroy != nullptr) {
			QueryParams.AddIgnoredActor(RoofSpawnDestroy);

		}
		if (WallSpawnDestroy != nullptr) {
			QueryParams.AddIgnoredActor(WallSpawnDestroy);

		}
	}
	// To run the query, you need a pointer to the current level, which you can get from an Actor with GetWorld()
	// UWorld()->LineTraceSingleByChannel runs a line trace and returns the first actor hit over the provided collision channel.
	GetWorld()->LineTraceSingleByChannel(Hit, start1, end1, TraceChannelProperty, QueryParams);



	// If the trace hit something, bBlockingHit will be true,
	// and its fields will be filled with detailed info about what was hit
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		traceHit = Hit.GetActor();
		WhereBuild = traceHit->GetActorLocation() - Hit.Location;
		if (Cast<ABuildSystem>(Hit.GetActor())) {
			CanUpgrate = true;
			UpgrateObject = Cast<ABuildSystem>(Hit.GetActor());

		}
		else {
			CanUpgrate = false;
			UpgrateObject = nullptr;

		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (traceHit->GetActorLocation()- GetActorLocation()).ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, (traceHit ->GetActorRotation()).ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, WhereBuild.ToString());

	}
	else {
		traceHit = nullptr;
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		PlayerInputComponent->BindAction("Increment", IE_Pressed, this, &AMyProjectCharacter::Increment);

		PlayerInputComponent->BindAction("OnBuildSystem", IE_Pressed, this, &AMyProjectCharacter::OnStartSystem);
		PlayerInputComponent->BindAction("OnBuildSystem", IE_Released, this, &AMyProjectCharacter::OnEndSystem);

		PlayerInputComponent->BindAction("OpenDoor", IE_Pressed, this, &AMyProjectCharacter::OpenDoor);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Move);
		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyProjectCharacter::LMB_click);
		PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyProjectCharacter::LMB_NotClick);
		PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AMyProjectCharacter::PickUp);
		PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AMyProjectCharacter::Drop);
		PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMyProjectCharacter::Reload);
		PlayerInputComponent->BindAction("Change", IE_Pressed, this, &AMyProjectCharacter::Change);
		PlayerInputComponent->BindAction("Repair", IE_Pressed, this, &AMyProjectCharacter::RepairGun);
		PlayerInputComponent->BindAction("Clean", IE_Pressed, this, &AMyProjectCharacter::CleanGun);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Look);

	}
}


void AMyProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMyProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyProjectCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

void AMyProjectCharacter::SetHasPistol(bool bNewHasPistol)
{
	bHasPistol = bNewHasPistol;

}

void AMyProjectCharacter::SetHasMallet(bool bNewHasMallet)
{
	bHasMallet = bNewHasMallet;

}

bool AMyProjectCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AMyProjectCharacter::PlaySoundEquip()
{
	if (EquipGunSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(this, EquipGunSound, GetActorLocation());
	}
}

void AMyProjectCharacter::FastCraft(int whatCraft)
{

	//GetWorld()->SpawnActor<AActor>(PickItem[0]->GetClass(),GetActorLocation(), GetActorRotation());
	if (whatCraft == 0) {
		ActorToCraft = GetWorld()->SpawnActor<AMalletItems>(FVector(0.f), FRotator(0.f));

		TArray<FString> CraftObjects;
		TArray<int> NeedNumCraft;

		NeedNumCraft.Add(1);
		CraftObjects.Add("MyWoodItems1_C");
		FindItem(CraftObjects, NeedNumCraft, 1, 1);


		if (Craft(CraftItem, RequiredNumForCraft, 1, 0)) {
				
			if (ActorToCraft != nullptr) {
				AddToInventory(ActorToCraft, 1);

			}

		}
			

	}
	
	if (whatCraft == 1) {
		ActorToCraft = GetWorld()->SpawnActor<AAk47>(FVector(0.f), FRotator(0.f));
		TArray<FString> CraftObjects;
		TArray<int> NeedNumCraft;

		NeedNumCraft.Add(4);
		CraftObjects.Add("MyWoodItems1_C");
		NeedNumCraft.Add(10);
		CraftObjects.Add("MyMetalItems_C");
		NeedNumCraft.Add(4);
		CraftObjects.Add("SpringInventoryItems");
		NeedNumCraft.Add(10);
		CraftObjects.Add("MyPowderInventoryItems_C");

		FindItem(CraftObjects, NeedNumCraft, 1, 1);

		if (Craft(CraftItem, RequiredNumForCraft, 1, 0)) {

			if (ActorToCraft != nullptr) {
				AddToInventory(ActorToCraft, 1);

			}

		}
	}
	if (whatCraft == 2) {
		ActorToCraft = GetWorld()->SpawnActor<ARevolverItems>(FVector(0.f), FRotator(0.f));
		TArray<FString> CraftObjects;
		TArray<int> NeedNumCraft;

		NeedNumCraft.Add(4);
		CraftObjects.Add("MyWoodItems1_C");
		NeedNumCraft.Add(10);
		CraftObjects.Add("MyMetalItems_C");
		NeedNumCraft.Add(4);
		CraftObjects.Add("SpringInventoryItems");
		NeedNumCraft.Add(10);
		CraftObjects.Add("MyPowderInventoryItems_C");

		FindItem(CraftObjects, NeedNumCraft, 1, 1);

		if (Craft(CraftItem, RequiredNumForCraft, 1, 0)) {

			if (ActorToCraft != nullptr) {
				AddToInventory(ActorToCraft, 1);

			}

		}
	}
	if (whatCraft==3) {
		ActorToCraft = GetWorld()->SpawnActor<AMakarov>(FVector(0.f), FRotator(0.f));
		TArray<FString> CraftObjects;
		TArray<int> NeedNumCraft;

		NeedNumCraft.Add(4);
		CraftObjects.Add("MyWoodItems1_C");
		NeedNumCraft.Add(10);
		CraftObjects.Add("MyMetalItems_C");
		NeedNumCraft.Add(4);
		CraftObjects.Add("SpringInventoryItems");
		NeedNumCraft.Add(10);
		CraftObjects.Add("MyPowderInventoryItems_C");

		FindItem(CraftObjects, NeedNumCraft, 1, 1);

		if (Craft(CraftItem, RequiredNumForCraft, 1, 0)) {

			if (ActorToCraft != nullptr) {
				AddToInventory(ActorToCraft, 1);

			}

		}
	}
	if (whatCraft == 4) {
		ActorToCraft = GetWorld()->SpawnActor<AStoneAxeInventoryItems>(FVector(0.f), FRotator(0.f));
		TArray<FString> CraftObjects;
		TArray<int> NeedNumCraft;

		NeedNumCraft.Add(2);
		CraftObjects.Add("MyWoodItems1_C");
		NeedNumCraft.Add(2);
		CraftObjects.Add("MyStoneItems_C");
		FindItem(CraftObjects, NeedNumCraft, 1, 1);

		if (Craft(CraftItem, RequiredNumForCraft, 1, 0)) {

			if (ActorToCraft != nullptr) {
				AddToInventory(ActorToCraft, 1);

			}

		}
	}
	if (whatCraft == 5) {
		
		ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));
		TArray<FString> CraftObjects;
		TArray<int> NeedNumCraft;

		NeedNumCraft.Add(3);
		CraftObjects.Add("MyWoodItems1_C");
		NeedNumCraft.Add(3);
		CraftObjects.Add("MyMetalItems_C");
		FindItem(CraftObjects, NeedNumCraft, 1, 1);
		if (Craft(CraftItem, RequiredNumForCraft, 1, 0)) {

			if (ActorToCraft != nullptr) {
				AddToInventory(ActorToCraft, 1);

			}

		}
	}

}

void AMyProjectCharacter::MakeEquipGun(int NumberOfInventoryItem)
{

	if (PickItem[NumberOfInventoryItem] != "StoneAxeInventoryItems" || PickItem[NumberOfInventoryItem] != "MalletItems" 
		|| PickItem[NumberOfInventoryItem] != "MetalAxeInventoryItems" || PickItem[NumberOfInventoryItem] != "Ak47" 
		|| PickItem[NumberOfInventoryItem] != "Makarov" || PickItem[NumberOfInventoryItem] != "Usp") {
		AInventoryItems* MakedEquipGun;
		MakedEquipGun=DeleteFromInventory(NumberOfInventoryItem);
		if (MakedEquipGun != nullptr) {
			PickUpGun(Cast<AHandItems>(MakedEquipGun));
			//MakedEquipGun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("PistolPoint"));

			MakedEquipGun->StopSymulatePhysics();

			
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("MakeEquipGun and not nullptr"));
		}
	
	}
}



void AMyProjectCharacter::FunctionForCraft(int HowMuchCraft)
{
	if (Craft(CraftItem, RequiredNumForCraft, HowMuchCraft, 0)) {
		//AddToInventoryByCraft("romaaddname", HowMuchCraft, IconWhatCraft);
		if (ActorToCraft != nullptr) {
			AddToInventory(ActorToCraft, HowMuchCraft);

		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("your actor is nullptr please check"));

		}
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("HowMuchCraft %i"), HowMuchCraft));

	}

	

}

AInventoryItems* AMyProjectCharacter::DeleteFromInventory(int whatDelete)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, PickItem[whatDelete]->GetClass()->GetName());

	if (PickItem[whatDelete] == "MyWoodItems1_C") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<AWoodItems>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "MyMetalItems_C") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<AMetalItems>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "MyStoneItems_C") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<AStoneItems>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "MyClothItems_C") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<AClothItems>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "MalletItems") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<AMalletItems>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "StoneAxeInventoryItems") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<AStoneAxeInventoryItems>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "MetalAxeInventoryItems") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "Ak47" || PickItem[whatDelete] == "Ak47_C") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<AAk47>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "RevolverItems" || PickItem[whatDelete] == "MyRevolverItems_C") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<ARevolverItems>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "Thompson" || PickItem[whatDelete] == "MyThompson_C") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<AThompson>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "Sg553Items" || PickItem[whatDelete] == "MySg553Items_C") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<ASg553Items>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "Makarov" || PickItem[whatDelete] == "MyMakarov_C") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<AMakarov>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	else if (PickItem[whatDelete] == "Usp" || PickItem[whatDelete] == "MyUsp_C") {

		DeletedActorFromInventory = GetWorld()->SpawnActor<AUsp>(GetActorLocation() + FVector(100, 0, 0), GetActorRotation());
	}
	if (DeletedActorFromInventory != nullptr) {
		//DeletedActorFromInventory->StartSymulatePhysics();
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("DeleteFromInventory"));
	
	
	
	
	PickItem.RemoveAt(whatDelete);
	Iconezzz.RemoveAt(whatDelete);
	PickItemNum.RemoveAt(whatDelete);
	return DeletedActorFromInventory;
}


void AMyProjectCharacter::LMB_click()
{
	IsFiring = true;

	ShotsInaRow = 0;
	DifferentX = 0;
	DifferentZ = 0;
	if (EquipGun != nullptr) {
		if (Cast<AGun>(EquipGun)) {
			CoefficientOfRecoil = Cast<AGun>(EquipGun)->GetCoefficientOfRecoil();
		}
	}
	LMB();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("StartFire"));

}

void AMyProjectCharacter::GetDataForCraft(int section, int numItem)
{
	if (ActorToCraft != nullptr) {
		ActorToCraft->Destroy();
		ActorToCraft = nullptr;
	}
	IconForCraft.Empty();
	NameForCraft.Empty();
	TArray<FString> CraftObjects;
	TArray<int> NeedNumCraft;
	if (section == 0) {
		if (numItem == 0) {
			
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			NameForCraft.Add("Mallet");
			NameForCraft.Add("it's mallet for building.");
			ActorToCraft = GetWorld()->SpawnActor<AMalletItems>(FVector(0.f), FRotator(0.f));
			
			
			NeedNumCraft.Add(1);
			CraftObjects.Add("MyWoodItems1_C");
			FindItem(CraftObjects, NeedNumCraft,1,1);
			/*IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Cloth.Cloth'"))));
			NameForCraft.Add("bandages");
			NameForCraft.Add("it's bandages for get healther.");

			NeedNumCraft.Add(4);
			CraftObjects.Add("MyClothItems_C");
			FindItem(CraftObjects, NeedNumCraft, 1, 1);*/
		}
		if (numItem == 1) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Stone.Stone'"))));
			NameForCraft.Add("Stone Hatchet");
			NameForCraft.Add("it's Stone Hatchet for making resources.");
			ActorToCraft = GetWorld()->SpawnActor<AStoneAxeInventoryItems>(FVector(0.f), FRotator(0.f));

			
			NeedNumCraft.Add(2);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(2);
			CraftObjects.Add("MyStoneItems_C");
			FindItem(CraftObjects, NeedNumCraft,1, 1);

		}
		if (numItem == 2) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			NameForCraft.Add("Hatchet");
			NameForCraft.Add("it's Hatchet for making resources.");
			ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(3);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(3);
			CraftObjects.Add("MyMetalItems_C");
			FindItem(CraftObjects, NeedNumCraft,1, 1);
		}
		if (numItem == 3) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Stone.Stone'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Cloth.Cloth'"))));


			NameForCraft.Add("Stone Pick Axe");
			NameForCraft.Add("it's Stone Pick Axe for making resources.");
			//ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));

			
			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(5);
			CraftObjects.Add("MyStoneItems_C");
			NeedNumCraft.Add(1);
			CraftObjects.Add("MyClothItems_C");
			FindItem(CraftObjects, NeedNumCraft,1, 1);

		}
		if (numItem == 4) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Cloth.Cloth'"))));


			NameForCraft.Add("Salvaged Hammer");
			NameForCraft.Add("it's Salvaged Hammer for making resources.");
			//ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(2);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(1);
			CraftObjects.Add("MyClothItems_C");
			FindItem(CraftObjects, NeedNumCraft, 1, 1);

		}
		if (numItem == 5) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Cloth.Cloth'"))));


			NameForCraft.Add("Salvaged Icepick");
			NameForCraft.Add("it's Salvaged Icepick for making resources.");
			//ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(4);
			CraftObjects.Add("MyClothItems_C");
			FindItem(CraftObjects, NeedNumCraft, 1, 1);

		}
		if (numItem == 6) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));


			NameForCraft.Add("Jackhammer");
			NameForCraft.Add("it's Jackhammer for making resources.");
			//ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);

		}
		if (numItem == 7) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));


			NameForCraft.Add("Chainsaw");
			NameForCraft.Add("it's Chainsaw for making resources.");
			//ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(4);
			CraftObjects.Add("MyStoneItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);

		}
	}
	if (section == 1) {
		if (numItem == 0) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Spring.Spring'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Powder.Powder'"))));


			NameForCraft.Add("AK47");
			NameForCraft.Add("it's AK47 for shooting.");
			ActorToCraft = GetWorld()->SpawnActor<AAk47>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(4);
			CraftObjects.Add("SpringInventoryItems");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyPowderInventoryItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);
		}
		if (numItem == 1) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Stone.Stone'"))));
			NameForCraft.Add("Stone Hatchet");
			NameForCraft.Add("it's Stone Hatchet for making resources.");


			NeedNumCraft.Add(2);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(2);
			CraftObjects.Add("MyStoneItems_C");
			FindItem(CraftObjects, NeedNumCraft, 1, 1);
		}
		if (numItem == 2) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));


			NameForCraft.Add("hand-loading");
			NameForCraft.Add("it's hand-loading for shooting.");
			//ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);
		}
		if (numItem == 3) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));


			NameForCraft.Add("Longsword");
			NameForCraft.Add("it's Longsword for making resources.");
			//ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);

		}
		if (numItem == 4) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Spring.Spring'"))));


			NameForCraft.Add("Shotgun");
			NameForCraft.Add("it's Shotgun for shooting.");
			ActorToCraft = GetWorld()->SpawnActor<AAk47>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(2);
			CraftObjects.Add("SpringInventoryItems");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);
		}
		if (numItem == 5) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Powder.Powder'"))));


			NameForCraft.Add("AK47");
			NameForCraft.Add("it's AK47 for shooting.");
			ActorToCraft = GetWorld()->SpawnActor<AAk47>(FVector(0.f), FRotator(0.f));



			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(4);
			CraftObjects.Add("MyPowderInventoryItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);
		}
		if (numItem == 6) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Powder.Powder'"))));


			NameForCraft.Add("AK47");
			NameForCraft.Add("it's AK47 for shooting.");
			ActorToCraft = GetWorld()->SpawnActor<AAk47>(FVector(0.f), FRotator(0.f));



			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(4);
			CraftObjects.Add("MyPowderInventoryItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);
		}
		if (numItem == 7) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Spring.Spring'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Powder.Powder'"))));


			NameForCraft.Add("Makarov");
			NameForCraft.Add("it's Makarov for shooting.");
			ActorToCraft = GetWorld()->SpawnActor<AMakarov>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(4);
			CraftObjects.Add("SpringInventoryItems");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyPowderInventoryItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);
		}
		if (numItem == 8) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Spring.Spring'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Powder.Powder'"))));


			NameForCraft.Add("AK47");
			NameForCraft.Add("it's AK47 for shooting.");
			ActorToCraft = GetWorld()->SpawnActor<AAk47>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(4);
			CraftObjects.Add("SpringInventoryItems");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyPowderInventoryItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);
		}

		if (numItem == 9) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Powder.Powder'"))));


			NameForCraft.Add("AK47");
			NameForCraft.Add("it's AK47 for shooting.");
			ActorToCraft = GetWorld()->SpawnActor<AAk47>(FVector(0.f), FRotator(0.f));



			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(4);
			CraftObjects.Add("MyPowderInventoryItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);
		}
		if (numItem == 10) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Spring.Spring'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Powder.Powder'"))));


			NameForCraft.Add("Sg553");
			NameForCraft.Add("it's Sg553 for shooting.");
			ActorToCraft = GetWorld()->SpawnActor<ASg553Items>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(4);
			CraftObjects.Add("SpringInventoryItems");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyPowderInventoryItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);
		}
		if (numItem == 11) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Wood.Wood'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Spring.Spring'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Powder.Powder'"))));


			NameForCraft.Add("Colt");
			NameForCraft.Add("it's Colt for shooting.");
			ActorToCraft = GetWorld()->SpawnActor<ARevolverItems>(FVector(0.f), FRotator(0.f));


			NeedNumCraft.Add(4);
			CraftObjects.Add("MyWoodItems1_C");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(4);
			CraftObjects.Add("SpringInventoryItems");
			NeedNumCraft.Add(10);
			CraftObjects.Add("MyPowderInventoryItems_C");

			FindItem(CraftObjects, NeedNumCraft, 1, 1);
		}
	}
	if (section == 2) {
		if (numItem == 0) {
			
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Cloth.Cloth'"))));
			NameForCraft.Add("bandages");
			NameForCraft.Add("it's bandages for get healther.");
			ActorToCraft = GetWorld()->SpawnActor<ABandagesInventoryItems>(FVector(0.f), FRotator(0.f));

			NeedNumCraft.Add(4);
			CraftObjects.Add("MyClothItems_C");
			FindItem(CraftObjects, NeedNumCraft, 1, 1);

		}
	}
	if (section == 3) {
		if (numItem == 0) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Cloth.Cloth'"))));


			NameForCraft.Add("Helmet");
			NameForCraft.Add("it's Helmet for ur self.");
			//ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));



			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(1);
			CraftObjects.Add("MyClothItems_C");
			FindItem(CraftObjects, NeedNumCraft, 1, 1);


		}
		if (numItem == 1) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Cloth.Cloth'"))));


			NameForCraft.Add("Facemak");
			NameForCraft.Add("it's Facemak for ur self.");
			//ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));



			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(1);
			CraftObjects.Add("MyClothItems_C");
			FindItem(CraftObjects, NeedNumCraft, 1, 1);


		}
		if (numItem == 2) {
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Cloth.Cloth'"))));


			NameForCraft.Add("Jacket");
			NameForCraft.Add("it's Jacket for ur self.");
			//ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));



			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(1);
			CraftObjects.Add("MyClothItems_C");
			FindItem(CraftObjects, NeedNumCraft, 1, 1);


		}
		if (numItem == 3) {

			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Cloth.Cloth'"))));


			NameForCraft.Add("Plate");
			NameForCraft.Add("it's Plate for ur self.");
			//ActorToCraft = GetWorld()->SpawnActor<AMetalAxeInventoryItems>(FVector(0.f), FRotator(0.f));



			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			NeedNumCraft.Add(1);
			CraftObjects.Add("MyClothItems_C");
			FindItem(CraftObjects, NeedNumCraft, 1, 1);


		}
	}
	if (section == 6) {
		if (numItem == 3) {

			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/ammo_rifle.ammo_rifle'"))));
			NameForCraft.Add("Ammo rifle");
			NameForCraft.Add("it's Ammo rifle for shooting.");
			ActorToCraft = GetWorld()->SpawnActor<AMyProjectProjectile>(FVector(0.f), FRotator(0.f));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, ActorToCraft->GetClass()->GetName()); 

			NeedNumCraft.Add(10);
			CraftObjects.Add("MyMetalItems_C");
			FindItem(CraftObjects, NeedNumCraft, 1, 1);

		}
	}
	if (section == 7) {
		if (numItem == 5) {

			IconForCraft.Add(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/Metal.Metal'"))));
			NameForCraft.Add("Spring");
			NameForCraft.Add("it's Spring for making something.");
			ActorToCraft = GetWorld()->SpawnActor<ASpringInventoryItems>(FVector(0.f), FRotator(0.f));

			NeedNumCraft.Add(5);
			CraftObjects.Add("MyMetalItems_C");
			FindItem(CraftObjects, NeedNumCraft, 1, 1);

		}
	}
}

bool AMyProjectCharacter::FindItem(TArray<FString> ObjectsCraft, TArray<int> NeedToCraft, int HowMuchCraft, bool GetCraftNum)
{
	bool CanCraft = true;

	if (GetCraftNum) {
		NumForCraft.Empty();
		RequiredNumForCraft.Empty();
	}

	for (int i = 0; i < ObjectsCraft.Num(); i++) {
		int NumOfMaterial = 0;

		TArray<int> index;
		for (int j = 0; j < PickItem.Num(); j++) {
			
				if (ObjectsCraft[i] == PickItem[j]) {
					NumOfMaterial += PickItemNum[j];
					index.Add(j);

				}
			
		}

		if (NumOfMaterial < NeedToCraft[i]* HowMuchCraft) {
			CanCraft = false;

		}
		if (GetCraftNum) {
			NumForCraft.Add(NumOfMaterial);
		}
		

	}
	if (GetCraftNum) {	
			RequiredNumForCraft = NeedToCraft;
			CraftItem = ObjectsCraft;
	}
	return CanCraft;
}

void AMyProjectCharacter::Increment()
{
	whatBuild++;
	if (whatBuild == 5) {
		whatBuild = 0;
	}
}
void AMyProjectCharacter::OnStartSystem()
{

}

void AMyProjectCharacter::OnEndSystem()
{

}




void AMyProjectCharacter::OpenDoor()
{
	if (Cast<Adoor>(traceHit)) {
		if (!Cast<Adoor>(traceHit)->IsOpen()) {
			Cast<Adoor>(traceHit)->OpenedDoor();
		}
		else {
			Cast<Adoor>(traceHit)->CloseDoor();
		}

	}
}
 
void AMyProjectCharacter::LMB_NotClick()
{
	IsFiring = false;
}

void AMyProjectCharacter::LMB()
{
	if (!IsMenuOpen) {

		if (isBuilding) {

			FVector loc;
			FRotator rot;
			FHitResult Hit;

			GetController()->GetPlayerViewPoint(loc, rot);

			FVector start = loc;
			FVector end = start + (rot.Vector() * 700);
			if (whatBuild == 0) {
				if (Cast<Afloor>(traceHit)) {
					if (WhereBuild.X > 0 && WhereBuild.Y > 0) {

						SpawnFloor(traceHit->GetActorLocation() + FVector(0.f, -300.f, 0.f), FRotator(0.f), true, true, false);

					}
					else if (WhereBuild.X < 0 && WhereBuild.Y > 0) {
						SpawnFloor(traceHit->GetActorLocation() + FVector(300.f, 0.f, 0.f), FRotator(0.f), true, true, false);

					}
					else if (WhereBuild.X < 0 && WhereBuild.Y < 0) {
						SpawnFloor(traceHit->GetActorLocation() + FVector(0.f, 300.f, 0.f), FRotator(0.f), true, true, false);

					}
					else {
						SpawnFloor(traceHit->GetActorLocation() + FVector(-300.f, 0.f, 0.f), FRotator(0.f), true, true, false);

					}

				}
				else {
					SpawnFloor(end, FRotator(0.f), true, true, false);
				}

			}
			else if (whatBuild == 1) {
				if (Cast<Afloor>(traceHit)) {
					if (WhereBuild.X > 0 && WhereBuild.Y > 0) {
						SpawnWall(traceHit->GetActorLocation() + FVector(0.f, -148.f, 235.f), FRotator(0.f, -90.f, 0.f), true, true, false);
					}
					else if (WhereBuild.X < 0 && WhereBuild.Y > 0) {

						SpawnWall(traceHit->GetActorLocation() + FVector(148.f, 0.f, 235.f), FRotator(0.f), true, true, false);
					}
					else if (WhereBuild.X < 0 && WhereBuild.Y < 0) {
						SpawnWall(traceHit->GetActorLocation() + FVector(0.f, 148.f, 235.f), FRotator(0.f, 90.f, 0.f), true, true, false);
					}
					else {
						SpawnWall(traceHit->GetActorLocation() + FVector(-148.f, 0.f, 235.f), FRotator(0.f, 180.f, 0.f), true, true, false);
					}
				}
				else {
					SpawnWall(end, FRotator(0.f), true, true, false);
				}
			}
			else if (whatBuild == 2) {
				if (Cast<Awall>(traceHit) || Cast<ADoorFrame>(traceHit)) {
					if ((traceHit->GetActorRotation() == FRotator(90.f, 90.f, 0.f) || traceHit->GetActorRotation() == FRotator(90.f, 90.f, 180.f)) || (traceHit->GetActorRotation() == Wall1 || traceHit->GetActorRotation() == Wall3)) {
						if ((traceHit->GetActorLocation() - GetActorLocation()).Y > 0) {
							SpawnRoof(traceHit->GetActorLocation() + FVector(0.f, -150.f, 150.f), FRotator(0.f), true, true, false);
						}
						else {
							SpawnRoof(traceHit->GetActorLocation() + FVector(0.f, 150.f, 150.f), FRotator(0.f), true, true, false);
						}
					}
					else {
						if ((traceHit->GetActorLocation() - GetActorLocation()).X > 0) {
							SpawnRoof(traceHit->GetActorLocation() + FVector(-150.f, 0.f, 150.f), FRotator(0.f), true, true, false);
						}
						else {
							SpawnRoof(traceHit->GetActorLocation() + FVector(150.f, 0.f, 150.f), FRotator(0.f), true, true, false);
						}

					}
				}


				else if (Cast<Aroof>(traceHit)) {
					if (WhereBuild.X > 0 && WhereBuild.Y > 0) {
						SpawnRoof(traceHit->GetActorLocation() + FVector(0.f, -300.f, 0.f), FRotator(0.f), true, true, false);
					}
					else if (WhereBuild.X < 0 && WhereBuild.Y > 0) {
						SpawnRoof(traceHit->GetActorLocation() + FVector(300.f, 0.f, 0.f), FRotator(0.f), true, true, false);
					}
					else if (WhereBuild.X < 0 && WhereBuild.Y < 0) {
						SpawnRoof(traceHit->GetActorLocation() + FVector(0.f, 300.f, 0.f), FRotator(0.f), true, true, false);
					}
					else {
						SpawnRoof(traceHit->GetActorLocation() + FVector(-300.f, 0.f, 0.f), FRotator(0.f), true, true, false);
					}
				}
				else {
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cant Build here!!!!!!!!!!!"));
				}
			}

			else if (whatBuild == 3) {
				if (Cast<Afloor>(traceHit)) {
					if (WhereBuild.X > 0 && WhereBuild.Y > 0) {


						SpawnDoorFrame(traceHit->GetActorLocation() + FVector(0.f, -148.f, 235.f), FRotator(0.f, 0.f, 0.f), true, true, false);

					}
					else if (WhereBuild.X < 0 && WhereBuild.Y > 0) {

						SpawnDoorFrame(traceHit->GetActorLocation() + FVector(148.f, 0.f, 235.f), FRotator(0.f, 90.f, 0.f), true, true, false);

					}
					else if (WhereBuild.X < 0 && WhereBuild.Y < 0) {
						SpawnDoorFrame(traceHit->GetActorLocation() + FVector(0.f, 148.f, 235.f), FRotator(0.f, 180.f, 0.f), true, true, false);

					}
					else {
						SpawnDoorFrame(traceHit->GetActorLocation() + FVector(-148.f, 0.f, 235.f), FRotator(0.f, 270.f, 0.f), true, true, false);

					}
				}
				else {
					SpawnDoorFrame(end, FRotator(0.f), true, true, false);
				}
			}
			else if (whatBuild == 4) {
				if (Cast<ADoorFrame>(traceHit)) {
					//тут есть баг, 
					if (traceHit->GetActorRotation() == Wall2 || traceHit->GetActorRotation() == Wall4) {
						SpawnDoor(traceHit->GetActorLocation(), FRotator(0.f, 90.f, 0.f), true, true, false);
					}
					else {
						SpawnDoor(traceHit->GetActorLocation(), FRotator(0.f, 0.f, 0.f), true, true, false);
					}

				}
				else {
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cant Build here!!!!!!!!!!!"));
				}
			}


		}
		else {
			if (IsFiring) {
				if (EquipGun != nullptr) {
					if (Cast<AGun>(EquipGun)) {
						//if (Cast<AGun>(EquipGun)->canFire()) {
							// try and fire a projectile
						//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fire"));

						UWorld* const World = GetWorld();
						if (World != NULL)
						{
							if (ProjectileClass != nullptr)
							{
								if (Cast<AGun>(EquipGun)->CurrentBulletInGun > 0) {

									FirstPersonCameraComponent->SetRelativeRotation(FirstPersonCameraComponent->GetRelativeRotation() + FRotator(0, 0, 1.2f));
									ShotsInaRow++;

									Cast<AGun>(EquipGun)->fire();

									if (ShotsInaRow % 7 == 0) {
										CoefficientOfRecoil = CoefficientOfRecoil * (-1);
										SprayEditHorizontal = -(Cast<AGun>(EquipGun)->GetSprayEditHorizontal());
									}

									if (ShotsInaRow % 4 == 0) {
										SprayEditHorizontal = Cast<AGun>(EquipGun)->GetSprayEditHorizontal() * 2;
									}
									else if (ShotsInaRow % 5 == 0) {
										SprayEditHorizontal = -(Cast<AGun>(EquipGun)->GetSprayEditHorizontal());
									}
									else if (ShotsInaRow % 3 == 0) {
										SprayEditHorizontal = -(Cast<AGun>(EquipGun)->GetSprayEditHorizontal()) / 2;
									}
									else {
										SprayEditHorizontal = 0.f;
									}

									FVector Start = FirstPersonCameraComponent->GetComponentLocation();

									FRotator ForwardVector = FirstPersonCameraComponent->GetComponentRotation();
									FVector End = (((FRotator(DifferentZ, DifferentX / 30.f, 0.f) + ForwardVector).Vector() * 8000.f) + Start);

									DifferentX += CoefficientOfRecoil;

									DifferentZ = ShotsInaRow / Cast<AGun>(EquipGun)->GetSprayEditVertical() - SprayEditHorizontal;

									FVector pos;
									FHitResult OutHit;

									APlayerController* PlayerController = Cast<APlayerController>(this->GetController());

									const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
									// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
									const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);


									if (FMath::RandRange(0.f, 100.f) < Cast<AGun>(EquipGun)->GetAccuracy()) {
										End += FVector(0.f, Cast<AGun>(EquipGun)->GetAccuracy() * 100, Cast<AGun>(EquipGun)->GetAccuracy() * 100);



									}

									FActorSpawnParameters ActorSpawnParams;
									ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

									if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
									{
										//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AAAAAAAAAAAAAA"));
										if (OutHit.bBlockingHit)
										{


											pos = OutHit.Location;
											//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OutHit.Location.ToString());

											pos -= SpawnLocation;
											SpawnedProjectile = World->SpawnActor<AMyProjectProjectile>(ProjectileClass, SpawnLocation, pos.Rotation(), ActorSpawnParams);

											CollisionParams.AddIgnoredActor(SpawnedProjectile);



											//DrawDebugLine(GetWorld(), Start, FRotator(0, 0, 0).RotateVector(End), FColor::Green, true);
											DrawDebugBox(GetWorld(), OutHit.Location, FVector(5.f), FColor::Red, true);
										}
									}
									else
									{
										//DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);
										pos = End - SpawnLocation;

										SpawnedProjectile = World->SpawnActor<AMyProjectProjectile>(ProjectileClass, SpawnLocation, pos.Rotation(), ActorSpawnParams);

										//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, End.Rotation().ToString());
										DrawDebugBox(GetWorld(), OutHit.Location, FVector(5.f), FColor::Red, true);
										CollisionParams.AddIgnoredActor(SpawnedProjectile);
										GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, SpawnedProjectile->GetActorLocation().ToString());
									}
									//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("zzzzzzzzzzz"));

									if (FireAnimation != nullptr && !bHasPistol)
									{
										// Get the animation object for the arms mesh
										UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
										if (AnimInstance != nullptr)
										{
											AnimInstance->Montage_Play(FireAnimation);
										}
									}


									if (FireEffectMuzzle != nullptr) {
										UNiagaraFunctionLibrary::SpawnSystemAttached(FireEffectMuzzle, FirstPersonCameraComponent, NAME_None, Cast<AGun>(EquipGun)->SpawnMuzzleFlashLocation, FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
									}
									//DrawDebugLine(GetWorld(), Start-FVector(10.f), End, FColor::Green, true);

									/*SpawnedProjectile->SpawnPosition = SpawnedProjectile->GetActorLocation();
									SpawnedProjectile->FireWeapon = EquipGun;*/
									if (Cast<AGun>(EquipGun)->IsAuto) {
										GetWorld()->GetTimerManager().SetTimer(TimeBtwShots, this, &AMyProjectCharacter::LMB, Cast<AGun>(EquipGun)->GetTimeBtwShots(), false);
									}
									if ((Cast<AGun>(EquipGun)->FireSound) != nullptr) {
										UGameplayStatics::PlaySoundAtLocation(this, Cast<AGun>(EquipGun)->FireSound, GetActorLocation());
									}
									if (bHasPistol) {
										if (FirePistolAnimation != nullptr)
										{
											// Get the animation object for the arms mesh
											UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
											if (AnimInstance != nullptr)
											{
												AnimInstance->Montage_Play(FirePistolAnimation);
											}
										}

									}

								
									/*	if (aaaa != nullptr) {
											aaaa->bbbb();
										}
										aaaa=GetWorld()->SpawnActor<AMyActor>(GetActorLocation()+FVector(100.f), GetActorRotation());*/
										// try and play the sound if specified
										/*if (FireSound != NULL)
										{
											UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
										}

										// try and play a firing animation if specified
										if (FireAnimation != NULL)
										{
											// Get the animation object for the arms mesh
											UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
											if (AnimInstance != NULL)
											{
												AnimInstance->Montage_Play(FireAnimation, 1.f);
											}
										}*/
								}
								else {
									if (ShootWithoutBullet != nullptr) {
										//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AAAAAAAAAAAAAA"));

										UGameplayStatics::PlaySoundAtLocation(this, ShootWithoutBullet, GetActorLocation());
									}
								}
							}
						}
						//	}
					}
					else if (Cast<AMetalAxeInventoryItems>(EquipGun)) {
						if (MeleeAttack != nullptr)
						{

							// Get the animation object for the arms mesh
							UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
							if (AnimInstance != nullptr)
							{
								AnimInstance->Montage_Play(MeleeAttack);
								//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AAAAAAAAAAAAAA"));

							}
						}
					}
				}
			}
		}
	}
}
	



void AMyProjectCharacter::PickUp()
{
	/**/FVector Start = FirstPersonCameraComponent->GetComponentLocation();

	FRotator ForwardVector = FirstPersonCameraComponent->GetComponentRotation();
	FVector End = (ForwardVector).Vector() * 1000.f + Start;
	FHitResult OutHit;
	CollisionParams.AddIgnoredActor(EquipGun);
	

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AAAAAAAAAAAAAA"));
		if (OutHit.bBlockingHit)
		{
			if (Cast<AHandItems>(OutHit.GetActor())) {
					PickUpGun(Cast<AHandItems>(OutHit.GetActor()));
					/*if (EquipGun == nullptr) {
						PickUpGun(Cast<AHandItems>(OutHit.GetActor()), "GripPoint");
						EquipGun = Cast<AHandItems>(OutHit.GetActor());
						IconEquipItem = EquipGun->ItemIcon;
						if (Cast<AGun>(EquipGun)) {
							Cast<AGun>(EquipGun)->equip();
							CoefficientOfRecoil = Cast<AGun>(EquipGun)->GetCoefficientOfRecoil();

						}


						EquipGun->SetActorRelativeLocation(EquipGun->VectorOfEquipPosition);
						EquipGun->SetActorRelativeRotation(EquipGun->RotatorOfEquipPosition);

					}
					else if (NoEquipGun == nullptr) {
						OutHit.GetActor()->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("NoEquipGun"));
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NoEquipGun"));

						PickUpGun(Cast<AHandItems>(OutHit.GetActor()), "NoEquipGun");
						NoEquipGun = Cast<AHandItems>(OutHit.GetActor());
						IconNoEquipItem = NoEquipGun->ItemIcon;
					}
					else {
						AddToInventory(EquipGun, 1);
						PickUpGun(Cast<AHandItems>(OutHit.GetActor()), "GripPoint");
						EquipGun = Cast<AHandItems>(OutHit.GetActor());
						IconEquipItem = EquipGun->ItemIcon;
						if (Cast<AGun>(EquipGun)) {
							Cast<AGun>(EquipGun)->equip();
							CoefficientOfRecoil = Cast<AGun>(EquipGun)->GetCoefficientOfRecoil();

						}


						EquipGun->SetActorRelativeLocation(EquipGun->VectorOfEquipPosition);
						EquipGun->SetActorRelativeRotation(EquipGun->RotatorOfEquipPosition);

					}*/
				
			}
			else if (Cast<AInventoryItems>(OutHit.GetActor())) {
				AddToInventory(OutHit.GetActor(),1);

			}


		}
	}
}

void AMyProjectCharacter::PickUpGun(AHandItems* ItemOfInventory)
{

	if (EquipGun == nullptr) {	
		if (Cast<AGun>(ItemOfInventory)) {
			GetWorld()->GetTimerManager().SetTimer(timerToPlayEquipSound, this, &AMyProjectCharacter::PlaySoundEquip, Get_Pistol->GetDefaultBlendInTime(), false);

		}
		
		if (Cast<AMetalAxeInventoryItems>(ItemOfInventory) || Cast<AMalletItems>(ItemOfInventory) || Cast<AMakarov>(ItemOfInventory) || Cast<AUsp>(ItemOfInventory) || Cast<ARevolverItems>(ItemOfInventory)) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PickUpGun"));
				ItemOfInventory->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("PistolPoint"));
				SetHasPistol(true);
				if (Cast<AMetalAxeInventoryItems>(ItemOfInventory) || Cast<AMalletItems>(ItemOfInventory)) {
					SetHasMallet(true);
				}
				else {
					SetHasMallet(false);
				}
				//	PickUpGun(Cast<AHandItems>(ItemOfInventory), "PistolPoint");
			
			if (Get_Pistol != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(Get_Pistol);
				}
			}
		}
		else {
			PickUpGun(Cast<AHandItems>(ItemOfInventory), "GripPoint");
			this->SetHasPistol(false);
			this->SetHasRifle(true);
			SetHasMallet(false);
			if (Get_Rifle != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(Get_Rifle);
				}
			}
		}
		EquipGun = Cast<AHandItems>(ItemOfInventory);
		IconEquipItem = EquipGun->ItemIcon;
		if (Cast<AGun>(EquipGun)) {
			Cast<AGun>(EquipGun)->equip();
			CoefficientOfRecoil = Cast<AGun>(EquipGun)->GetCoefficientOfRecoil();

		}
		if (Cast<AMalletItems>(EquipGun)) {
			isBuilding = 1;
			OnBuildSystem();
		}
		if (!Cast<AMalletItems>(EquipGun)) {
			isBuilding = 0;
		}
		if (Cast<AMetalAxeInventoryItems>(ItemOfInventory) || Cast<AMalletItems>(ItemOfInventory) || Cast<AMakarov>(ItemOfInventory) || Cast<AUsp>(ItemOfInventory) || Cast<ARevolverItems>(ItemOfInventory)) {



		}
		else {
			EquipGun->SetActorScale3D(FVector(0.8, 0.8, 0.8));


		}
		EquipGun->SetActorRelativeRotation(EquipGun->RotatorOfEquipPosition);
		EquipGun->SetActorRelativeLocation(EquipGun->VectorOfEquipPosition);


	}
	else if (NoEquipGun == nullptr) {
		//this->SetHasRifle(false);
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NoEquipGun"));

		PickUpGun(Cast<AHandItems>(ItemOfInventory), "NoEquipGun");
		NoEquipGun = Cast<AHandItems>(ItemOfInventory);
		IconNoEquipItem = NoEquipGun->ItemIcon;
	}
	else if(NoEquipGun != nullptr || EquipGun != nullptr) {
		if (Cast<AGun>(ItemOfInventory)) {
			GetWorld()->GetTimerManager().SetTimer(timerToPlayEquipSound, this, &AMyProjectCharacter::PlaySoundEquip, Get_Pistol->GetDefaultBlendInTime(), false);

		}
		this->SetHasRifle(true);
		if (Cast<AMetalAxeInventoryItems>(ItemOfInventory) || Cast<AMalletItems>(ItemOfInventory) || Cast<AMakarov>(ItemOfInventory) || Cast<AUsp>(ItemOfInventory) || Cast<ARevolverItems>(ItemOfInventory)) {
			PickUpGun(Cast<AHandItems>(ItemOfInventory), "PistolPoint");
			this->SetHasPistol(true);
			if (Cast<AMalletItems>(ItemOfInventory)) {
				SetHasMallet(true);
			}
			else {
				SetHasMallet(false);
			}
			if (Get_Pistol != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(Get_Pistol);
				}
			}
		}
		else {
			PickUpGun(Cast<AHandItems>(ItemOfInventory), "GripPoint");
			this->SetHasPistol(false);
			SetHasMallet(false);
			if (Get_Rifle != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(Get_Rifle);
				}
			}

		}
		
		EquipGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquipGun->SetActorHiddenInGame(true);
		AddToInventory(EquipGun, 1);
		
		EquipGun = Cast<AHandItems>(ItemOfInventory);
		IconEquipItem = EquipGun->ItemIcon;
		if (Cast<AGun>(EquipGun)) {
			Cast<AGun>(EquipGun)->equip();
			CoefficientOfRecoil = Cast<AGun>(EquipGun)->GetCoefficientOfRecoil();

		}
		if (!Cast<AMalletItems>(EquipGun)) {
			isBuilding = 0;
		}
		if (Cast<AMalletItems>(EquipGun)) {
			isBuilding = 1;
			OnBuildSystem();
		}
		EquipGun->SetActorRelativeLocation(EquipGun->VectorOfEquipPosition);
		EquipGun->SetActorRelativeRotation(EquipGun->RotatorOfEquipPosition);

	}
}



void AMyProjectCharacter::AddToInventory(AActor* InventoryActor, int HowMuchAdd)
{
	bool IsDouble = 0;
	int DoubleElement;
	if (Cast<AInventoryItems>(InventoryActor)->AddToInventorySound != nullptr) {
		UGameplayStatics::SpawnSoundAttached(Cast<AInventoryItems>(InventoryActor)->AddToInventorySound, GetMesh());

	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, InventoryActor->GetClass()->GetName());
	//if (Cast<AInventoryItems>(InventoryActor)->RetCanStack()) {
	for (int i = 0; i < PickItem.Num(); i++) {
		
		if (PickItem[i] == InventoryActor->GetClass()->GetName()) {
			IsDouble = true;
			DoubleElement = i;
			

		}
	}
	if (IsDouble) {
		if (Cast<AInventoryItems>(InventoryActor)->GetCanStack()) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CanStack"));

			if (PickItemNum[DoubleElement] < Cast<AInventoryItems>(InventoryActor)->GetMaxStack()) {

				PickItemNum[DoubleElement] = PickItemNum[DoubleElement] + HowMuchAdd * Cast<AInventoryItems>(InventoryActor)->NumItemInOne;
				InventoryActor->Destroy();
			}
			else {
				if (PickItem.Num() < 200) {
					PickItem.Add(Cast<AInventoryItems>(InventoryActor)->GetClass()->GetName());
					PickItemNum.Add(HowMuchAdd * Cast<AInventoryItems>(InventoryActor)->NumItemInOne);
					Iconezzz.Add(Cast<AInventoryItems>(InventoryActor)->ItemIcon);
					InventoryActor->Destroy();
				}
			}
			
		}
		else {



			if (PickItem.Num() < 200) {
				PickItem.Add(Cast<AInventoryItems>(InventoryActor)->GetClass()->GetName());
				PickItemNum.Add(HowMuchAdd * Cast<AInventoryItems>(InventoryActor)->NumItemInOne);
				Iconezzz.Add(Cast<AInventoryItems>(InventoryActor)->ItemIcon);		
				InventoryActor->Destroy();

			}
		}
	}
	else {
		if (PickItem.Num() < 200) {
			PickItem.Add(Cast<AInventoryItems>(InventoryActor)->GetClass()->GetName());
			PickItemNum.Add(HowMuchAdd * Cast<AInventoryItems>(InventoryActor)->NumItemInOne);
			Iconezzz.Add(Cast<AInventoryItems>(InventoryActor)->ItemIcon);
		}
		Cast<AInventoryItems>(InventoryActor)->Destroed();

	}
}


void AMyProjectCharacter::OnBuildSystem()
{
	if (isBuilding) {


		FVector loc;
		FRotator rot;
		FHitResult Hit;

		GetController()->GetPlayerViewPoint(loc, rot);
		FVector start = loc;
		FVector end = start + (rot.Vector() * 700);

		if (whatBuild == 0) {
			if (Cast<Afloor>(traceHit)) {
				if (WhereBuild.X > 0 && WhereBuild.Y > 0) {

					SpawnFloor(traceHit->GetActorLocation() + FVector(0.f, -300.f, 0.f), FRotator(0.f), false, false, true);

				}
				else if (WhereBuild.X < 0 && WhereBuild.Y > 0) {
					SpawnFloor(traceHit->GetActorLocation() + FVector(300.f, 0.f, 0.f), FRotator(0.f), false, false, true);

				}
				else if (WhereBuild.X < 0 && WhereBuild.Y < 0) {
					SpawnFloor(traceHit->GetActorLocation() + FVector(0.f, 300.f, 0.f), FRotator(0.f), false, false, true);

				}
				else {
					SpawnFloor(traceHit->GetActorLocation() + FVector(-300.f, 0.f, 0.f), FRotator(0.f), false, false, true);

				}

			}
			else {
				SpawnFloor(end, FRotator(0.f), false, false, true);
			}

		}
		else if (whatBuild == 1) {
			if (Cast<Afloor>(traceHit)) {
				if (WhereBuild.X > 0 && WhereBuild.Y > 0) {
					SpawnWall(traceHit->GetActorLocation() + FVector(0.f, -148.f, 235.f), FRotator(0.f, -90.f, 0.f), false, false, true);
				}
				else if (WhereBuild.X < 0 && WhereBuild.Y > 0) {

					SpawnWall(traceHit->GetActorLocation() + FVector(148.f, 0.f, 235.f), FRotator(0.f), false, false, true);
				}
				else if (WhereBuild.X < 0 && WhereBuild.Y < 0) {
					SpawnWall(traceHit->GetActorLocation() + FVector(0.f, 148.f, 235.f), FRotator(0.f, 90.f, 0.f), false, false, true);
				}
				else {
					SpawnWall(traceHit->GetActorLocation() + FVector(-148.f, 0.f, 235.f), FRotator(0.f, 180.f, 0.f), false, false, true);
				}
			}
			else {
				SpawnWall(end, FRotator(0.f), false, false, true);
			}
		}
		else if (whatBuild == 2) {
			if (Cast<Awall>(traceHit) || Cast<ADoorFrame>(traceHit)) {
				if ((traceHit->GetActorRotation() == FRotator(90.f, 90.f, 0.f) || traceHit->GetActorRotation() == FRotator(90.f, 90.f, 180.f)) || (traceHit->GetActorRotation() == Wall1 || traceHit->GetActorRotation() == Wall3)) {
					if ((traceHit->GetActorLocation() - GetActorLocation()).Y > 0) {
						SpawnRoof(traceHit->GetActorLocation() + FVector(0.f, -150.f, 150.f), FRotator(0.f), false, false, true);
					}
					else {
						SpawnRoof(traceHit->GetActorLocation() + FVector(0.f, 150.f, 150.f), FRotator(0.f), false, false, true);
					}
				}
				else {
					if ((traceHit->GetActorLocation() - GetActorLocation()).X > 0) {
						SpawnRoof(traceHit->GetActorLocation() + FVector(-150.f, 0.f, 150.f), FRotator(0.f), false, false, true);
					}
					else {
						SpawnRoof(traceHit->GetActorLocation() + FVector(150.f, 0.f, 150.f), FRotator(0.f), false, false, true);
					}

				}
			}


			else if (Cast<Aroof>(traceHit)) {
				if (WhereBuild.X > 0 && WhereBuild.Y > 0) {
					SpawnRoof(traceHit->GetActorLocation() + FVector(0.f, -300.f, 0.f), FRotator(0.f), false, false, true);
				}
				else if (WhereBuild.X < 0 && WhereBuild.Y > 0) {
					SpawnRoof(traceHit->GetActorLocation() + FVector(300.f, 0.f, 0.f), FRotator(0.f), false, false, true);
				}
				else if (WhereBuild.X < 0 && WhereBuild.Y < 0) {
					SpawnRoof(traceHit->GetActorLocation() + FVector(0.f, 300.f, 0.f), FRotator(0.f), false, false, true);
				}
				else {
					SpawnRoof(traceHit->GetActorLocation() + FVector(-300.f, 0.f, 0.f), FRotator(0.f), false, false, true);
				}
			}

			else {
				SpawnRoof(end, FRotator(0.f), false, false, true);
				if (RoofSpawnDestroy != nullptr) {
					RoofSpawnDestroy->SetRedMesh();
				}
				//redmesh
			}
		}

		else if (whatBuild == 3) {
			if (Cast<Afloor>(traceHit)) {
				if (WhereBuild.X > 0 && WhereBuild.Y > 0) {
					SpawnDoorFrame(traceHit->GetActorLocation() + FVector(0.f, -148.f, 235.f), FRotator(0.f, 0.f, 0.f), false, false, true);
					Wall1 = DoorFrameSpawnDestroy->GetActorRotation();
				}
				else if (WhereBuild.X < 0 && WhereBuild.Y > 0) {

					SpawnDoorFrame(traceHit->GetActorLocation() + FVector(148.f, 0.f, 235.f), FRotator(0.f, 90.f, 0.f), false, false, true);
					Wall2 = DoorFrameSpawnDestroy->GetActorRotation();
				}
				else if (WhereBuild.X < 0 && WhereBuild.Y < 0) {
					SpawnDoorFrame(traceHit->GetActorLocation() + FVector(0.f, 148.f, 235.f), FRotator(0.f, 180.f, 0.f), false, false, true);
					Wall3 = DoorFrameSpawnDestroy->GetActorRotation();
				}
				else {
					SpawnDoorFrame(traceHit->GetActorLocation() + FVector(-148.f, 0.f, 235.f), FRotator(0.f, 270.f, 0.f), false, false, true);
					Wall4 = DoorFrameSpawnDestroy->GetActorRotation();
				}
			}
			else {
				SpawnDoorFrame(end, FRotator(0.f), false, false, true);
			}
		}
		else if (whatBuild == 4) {
			if (Cast<ADoorFrame>(traceHit)) {
				if (traceHit->GetActorRotation() == Wall2 || traceHit->GetActorRotation() == Wall4) {
					SpawnDoor(traceHit->GetActorLocation(), FRotator(0.f, 90.f, 0.f), false, false, true);
				}
				else {
					SpawnDoor(traceHit->GetActorLocation(), FRotator(0.f, 0.f, 0.f), false, false, true);
				}
			}
			else {
				SpawnDoor(end, FRotator(0.f), false, false, true);
				if (DoorSpawnDestroy != nullptr) {
					DoorSpawnDestroy->SetRedMesh();
				}


			}
		}
	}
}

	/*if (OverlapGun.Num() != 0) {
		this->SetHasRifle(true);
		if (EquipGun == nullptr) {
			PickUpGun(OverlapGun[0], "GripPoint");
			EquipGun = OverlapGun[0];
			EquipGun->equip();
			CoefficientOfRecoil = EquipGun->GetCoefficientOfRecoil();
		}
		else {
			PickUpGun(OverlapGun[0], "NoEquipGun");
			NoEquipGun = OverlapGun[0];
		}

		OverlapGun.RemoveAt(0);
	}
	*/


void AMyProjectCharacter::PickUpGun(AHandItems* Over, FName nameSocket)
{
	Over->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), nameSocket);
	Over->StopSymulatePhysics();

}

void AMyProjectCharacter::Drop()
{
	if (isBuilding) {
		if (UpgrateObject != nullptr) {
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Upgrate3"));
		
					if (UpgrateObject->levelOfMaterial == 1) {
						FString CraftObject = TEXT("MyStoneItems_C");
						TArray<FString> CraftObjects;
						TArray<int> NeedNumCraft;
						NeedNumCraft.Add(1);
						CraftObjects.Add(CraftObject);
						if (Craft(CraftObjects, NeedNumCraft,1,0)) {
							UpgrateObject->Upgrate();
						}
					}
					else if(UpgrateObject->levelOfMaterial == 2){
						FString CraftObject = TEXT("MyMetalItems_C");
						TArray<FString> CraftObjects;
						TArray<int> NeedNumCraft;
						NeedNumCraft.Add(1);
						CraftObjects.Add(CraftObject);
						if (Craft(CraftObjects, NeedNumCraft,1,0)) {
							UpgrateObject->Upgrate();
						}
					}


		
			}
	}
	
	else if (EquipGun != nullptr) {
		if (Cast<AGun>(EquipGun)) {

			//this->SetHasRifle(false);
			this->SetHasPistol(false);


			Cast<AGun>(EquipGun)->unequip();
			EquipGun->StartSymulatePhysics();
			EquipGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			OverlapGun.AddUnique(EquipGun);
			EquipGun = nullptr;
		}

	}

}

void AMyProjectCharacter::RepairGun()
{
	if (EquipGun != nullptr) {
		if (Cast<AGun>(EquipGun)) {
			Cast<AGun>(EquipGun)->RepairGun();
		}
	}
}

void AMyProjectCharacter::CleanGun()
{
	if (EquipGun != nullptr) {
		if (Cast<AGun>(EquipGun)) {
			Cast<AGun>(EquipGun)->CleanWeapon();
		}
	}
}



void AMyProjectCharacter::Reload()
{
	if (EquipGun != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("A"));


		if (Cast<AGun>(EquipGun)) {
			TArray<FString> CraftObjects;
			TArray<int> NeedNumCraft;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AA"));

			NeedNumCraft.Add(8);
			CraftObjects.Add("MyProjectProjectile");
			if (Craft(CraftObjects, NeedNumCraft, 1, 1)) {
				if (ReloadPistol != nullptr)
				{
					// Get the animation object for the arms mesh
					UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
					if (AnimInstance != nullptr)
					{
						AnimInstance->Montage_Play(ReloadPistol);
					}
				}
				if (ReloadPistolSound != nullptr) {

					UGameplayStatics::PlaySoundAtLocation(this, ReloadPistolSound, GetActorLocation());

				}
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AAAA"));

				Cast<AGun>(EquipGun)->reload();
			}

			
		}
	}
	GetArrElem();
}

void AMyProjectCharacter::Change()
{
	if (NoEquipGun != nullptr && EquipGun != nullptr) {
		if (Cast<AGun>(NoEquipGun)) {
			GetWorld()->GetTimerManager().SetTimer(timerToPlayEquipSound, this, &AMyProjectCharacter::PlaySoundEquip, Get_Pistol->GetDefaultBlendInTime(), false);

		}
		if (Cast<AMetalAxeInventoryItems>(NoEquipGun) || Cast<AMalletItems>(NoEquipGun) || Cast<AMakarov>(NoEquipGun) || Cast<AUsp>(NoEquipGun) || Cast<ARevolverItems>(NoEquipGun)) {
			PickUpGun(Cast<AHandItems>(NoEquipGun), "PistolPoint");
			this->SetHasPistol(true);
			if (Cast<AMetalAxeInventoryItems>(NoEquipGun) || Cast<AMalletItems>(NoEquipGun)) {
				SetHasMallet(true);
			}
			else {
				SetHasMallet(false);
			}
			if (Get_Pistol != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(Get_Pistol);
				}
			}
		}
		
	
		else {
			NoEquipGun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
			this->SetHasPistol(false);
			this->SetHasRifle(true);
			SetHasMallet(false);
			if (Get_Rifle != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(Get_Rifle);
				}
			}
		}
		

		EquipGun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("NoEquipGun"));
		BtwGun = EquipGun;
		EquipGun = NoEquipGun;
		NoEquipGun = BtwGun;
		if (Cast<AGun>(EquipGun)) {
			Cast<AGun>(EquipGun)->equip();
		}
		if (Cast<AGun>(NoEquipGun)) {
			Cast<AGun>(NoEquipGun)->unequip();
		}
		if (Cast<AMalletItems>(EquipGun)) {
			isBuilding = 1;
			OnBuildSystem();
		}
		EquipGun->SetActorRelativeLocation(EquipGun->VectorOfEquipPosition);
		EquipGun->SetActorRelativeRotation(EquipGun->RotatorOfEquipPosition);
		if (!Cast<AMalletItems>(EquipGun)) {
			isBuilding = 0;
		}
		IconEquipItem = EquipGun->ItemIcon;
		IconNoEquipItem = NoEquipGun->ItemIcon;
	}
	else if (NoEquipGun == nullptr && EquipGun != nullptr) {
		//this->SetHasRifle(false);
		SetHasPistol(false);
		SetHasRifle(false);
		SetHasMallet(false);
		EquipGun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("NoEquipGun"));
		NoEquipGun = EquipGun;
		if (Cast<AGun>(NoEquipGun)) {
			Cast<AGun>(NoEquipGun)->unequip();
		}
		isBuilding = 0;
		
		EquipGun = nullptr;
		IconNoEquipItem = NoEquipGun->ItemIcon;
		IconEquipItem = nullptr;
	}
	else if (NoEquipGun != nullptr && EquipGun == nullptr) {
		if (Cast<AGun>(NoEquipGun)) {
			GetWorld()->GetTimerManager().SetTimer(timerToPlayEquipSound, this, &AMyProjectCharacter::PlaySoundEquip, Get_Pistol->GetDefaultBlendInTime(), false);

		}
		if (Cast<AMetalAxeInventoryItems>(NoEquipGun) || Cast<AMalletItems>(NoEquipGun) || Cast<AMakarov>(NoEquipGun) || Cast<AUsp>(NoEquipGun) || Cast<ARevolverItems>(NoEquipGun)) {
			PickUpGun(Cast<AHandItems>(NoEquipGun), "PistolPoint");
			this->SetHasPistol(true);
			if (Cast<AMetalAxeInventoryItems>(NoEquipGun) || Cast<AMalletItems>(NoEquipGun)) {
				SetHasMallet(true);
			}
			else {
				SetHasMallet(false);
			}
			if (Get_Pistol != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(Get_Pistol);
				}
			}
		}
		else {
			NoEquipGun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
			this->SetHasPistol(false);
			this->SetHasRifle(true);
			

			if (Get_Rifle != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = this->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(Get_Rifle);
				}
			}
		}
		this->SetHasRifle(true);

		EquipGun = NoEquipGun;
		if (Cast<AGun>(EquipGun)) {
			Cast<AGun>(EquipGun)->unequip();
		}
		NoEquipGun = nullptr;
		if (Cast<AMalletItems>(EquipGun)) {
			isBuilding = 1;
			OnBuildSystem();
		}
		EquipGun->SetActorRelativeLocation(EquipGun->VectorOfEquipPosition);
		EquipGun->SetActorRelativeRotation(EquipGun->RotatorOfEquipPosition);
		IconEquipItem = EquipGun->ItemIcon;
		IconNoEquipItem = nullptr;

	}

}

void AMyProjectCharacter::GetArrElem()
{
	if (PickItem.Num()!=0) {
		for (int i = 0; i < PickItem.Num(); i++) {
			
		}
	}
}

bool AMyProjectCharacter::Craft(TArray<FString> ObjectsCraft, TArray<int> NeedToCraft, int HowMuchCraft, bool GetCraftNum)
{
	bool CanCraft = FindItem(ObjectsCraft, NeedToCraft, HowMuchCraft, GetCraftNum);
	
	
	if (CanCraft) {
		int NumOfMaterial = 0;
		TArray<int> index;
		for (int i = 0; i < ObjectsCraft.Num(); i++) {

			for (int j = 0; j < PickItem.Num(); j++) {
				
					if (ObjectsCraft[i] == PickItem[j]) {
						NumOfMaterial += PickItemNum[j];
						index.Add(j);

					}
				
				
			}
			int ChangedNeedToCraft = NeedToCraft[i] * HowMuchCraft;
			while (ChangedNeedToCraft != 0) {
				if (ChangedNeedToCraft >= PickItemNum[index.Last()]) {
					ChangedNeedToCraft -= PickItemNum[index.Last()];
					PickItemNum.RemoveAt(index.Last());
					PickItem.RemoveAt(index.Last());
					Iconezzz.RemoveAt(index.Last());
					index.RemoveAt(index.Num() - 1);
				}
				else {
					PickItemNum[index.Last()] -= ChangedNeedToCraft;
					ChangedNeedToCraft = 0;
				}
			}
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("CantCraft"));
		for (int j = 0; j < PickItem.Num(); j++) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("PickItemNum  %i"), PickItem.Num()));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("PickItemNum  %i"), PickItemNum[j]));
			
			
		}
	}
	return CanCraft;
}


void AMyProjectCharacter::SpawnRoof(FVector location, FRotator rotation, bool ShouldOverlap, bool ShouldDestroy, bool ShouldRespawn)
{
	if (ShouldRespawn) {
		RoofSpawnDestroy = GetWorld()->SpawnActor<Aroof>(location, rotation);
		RoofSpawnDestroy->SetGreenMesh();
	}
	else {
		RoofSpawn = GetWorld()->SpawnActor<Aroof>(location, rotation);
	}

	if (ShouldDestroy) {
		TArray<FString> CraftObjects;
		TArray<int> NeedNumCraft;

		NeedNumCraft.Add(1);
		CraftObjects.Add("MyWoodItems1_C");
		FindItem(CraftObjects, NeedNumCraft, 1, 1);

		if (!Craft(CraftItem, RequiredNumForCraft, 1, 0)) {
			RoofSpawn->Destr();

		}
	}

	if (!ShouldOverlap) {
		RoofSpawnDestroy->StopOverlap();

	}
	if (ShouldDestroy) {
		RoofSpawn = nullptr;
	}
	if (ShouldRespawn) {
		GetWorld()->GetTimerManager().SetTimer(timerToShot, this, &AMyProjectCharacter::OnDestroyRoof, 0.1f, false);
	}
	if (ShouldDestroy) {
		if (WoodSound != nullptr) {
			UGameplayStatics::PlaySoundAtLocation(this, WoodSound, GetActorLocation());
		}
	}
}

void AMyProjectCharacter::SpawnWall(FVector location, FRotator rotation, bool ShouldOverlap, bool ShouldDestroy, bool ShouldRespawn)
{
	if (ShouldRespawn) {
		WallSpawnDestroy = GetWorld()->SpawnActor<Awall>(location, rotation);
		WallSpawnDestroy->SetGreenMesh();
	}
	else {
		WallSpawn = GetWorld()->SpawnActor<Awall>(location, rotation);
	}

	if (ShouldDestroy) {

		TArray<FString> CraftObjects;
		TArray<int> NeedNumCraft;

		NeedNumCraft.Add(1);
		CraftObjects.Add("MyWoodItems1_C");
		FindItem(CraftObjects, NeedNumCraft, 1, 1);

		if (!Craft(CraftItem, RequiredNumForCraft, 1, 0)) {
			WallSpawn->Destr();

		}
		
	}

	if (!ShouldOverlap) {
		WallSpawnDestroy->StopOverlap();

	}
	if (ShouldDestroy) {
		if (WoodSound != nullptr) {
			UGameplayStatics::PlaySoundAtLocation(this, WoodSound, GetActorLocation());
		}
	}
	if (ShouldRespawn) {
		GetWorld()->GetTimerManager().SetTimer(timerToShot, this, &AMyProjectCharacter::OnDestroyWall, 0.1f, false);
	}
	
}

void AMyProjectCharacter::SpawnDoorFrame(FVector location, FRotator rotation, bool ShouldOverlap, bool ShouldDestroy, bool ShouldRespawn)
{
	if (ShouldRespawn) {
		DoorFrameSpawnDestroy = GetWorld()->SpawnActor<ADoorFrame>(location, rotation);
		DoorFrameSpawnDestroy->SetGreenMesh();

	}
	else {
		DoorFrameSpawn = GetWorld()->SpawnActor<ADoorFrame>(location, rotation);
	}

	if (ShouldDestroy) {
		TArray<FString> CraftObjects;
		TArray<int> NeedNumCraft;

		NeedNumCraft.Add(1);
		CraftObjects.Add("MyWoodItems1_C");
		FindItem(CraftObjects, NeedNumCraft, 1, 1);

		if (!Craft(CraftItem, RequiredNumForCraft, 1, 0)) {
			DoorFrameSpawn->Destr();

		}
	}

	if (!ShouldOverlap) {
		DoorFrameSpawnDestroy->StopOverlap();

	}
	if (ShouldDestroy) {
		DoorFrameSpawn = nullptr;
	}
	if (ShouldRespawn) {
		GetWorld()->GetTimerManager().SetTimer(timerToShot, this, &AMyProjectCharacter::OnDestroyDoorFrame, 0.1f, false);
	}
	if (ShouldDestroy) {
		if (WoodSound != nullptr) {
			UGameplayStatics::PlaySoundAtLocation(this, WoodSound, GetActorLocation());
		}
	}
}

void AMyProjectCharacter::SpawnDoor(FVector location, FRotator rotation, bool ShouldOverlap, bool ShouldDestroy, bool ShouldRespawn)
{
	if (ShouldRespawn) {
		DoorSpawnDestroy = GetWorld()->SpawnActor<Adoor>(location, rotation);
		DoorSpawnDestroy->SetGreenMesh();
	}
	else {
		DoorSpawn = GetWorld()->SpawnActor<Adoor>(location, rotation);

	}

	if (ShouldDestroy) {
		TArray<FString> CraftObjects;
		TArray<int> NeedNumCraft;

		NeedNumCraft.Add(1);
		CraftObjects.Add("MyWoodItems1_C");
		FindItem(CraftObjects, NeedNumCraft, 1, 1);

		if (!Craft(CraftItem, RequiredNumForCraft, 1, 0)) {
			DoorSpawn->Destr();

		}
	}

	if (!ShouldOverlap) {
		DoorSpawnDestroy->StopOverlap();

	}
	if (ShouldDestroy) {
		DoorSpawn = nullptr;
	}
	if (ShouldRespawn) {
		GetWorld()->GetTimerManager().SetTimer(timerToShot, this, &AMyProjectCharacter::OnDestroyDoor, 0.1f, false);
	}
	if (ShouldDestroy) {
		if (WoodSound != nullptr) {
			UGameplayStatics::PlaySoundAtLocation(this, WoodSound, GetActorLocation());
		}
	}
}

void AMyProjectCharacter::SpawnFloor(FVector location, FRotator rotation, bool ShouldOverlap, bool ShouldDestroy, bool ShouldRespawn)
{
	if (ShouldRespawn) {
		FloorSpawnDestroy = GetWorld()->SpawnActor<Afloor>(location, rotation);
		FloorSpawnDestroy->SetGreenMesh();


	}
	else {
		FloorSpawn = GetWorld()->SpawnActor<Afloor>(location, rotation);
	}

	if (ShouldDestroy) {
		TArray<FString> CraftObjects;
		TArray<int> NeedNumCraft;

		NeedNumCraft.Add(1);
		CraftObjects.Add("MyWoodItems1_C");
		FindItem(CraftObjects, NeedNumCraft, 1, 1);

		if (!Craft(CraftItem, RequiredNumForCraft, 1, 0)) {
			FloorSpawn->Destr();

		}
		if (ShouldDestroy) {
			if (WoodSound != nullptr) {
				UGameplayStatics::PlaySoundAtLocation(this, WoodSound, GetActorLocation());
			}
		}
	}

	if (!ShouldOverlap) {
		FloorSpawnDestroy->StopOverlap();

	}
	if (ShouldDestroy) {
		FloorSpawn = nullptr;
	}
	if (ShouldRespawn) {
		GetWorld()->GetTimerManager().SetTimer(timerToShot, this, &AMyProjectCharacter::OnDestroyFloor, 0.1f, false);
	}
	if (ShouldDestroy) {
		if (WoodSound != nullptr) {
			UGameplayStatics::PlaySoundAtLocation(this, WoodSound, GetActorLocation());
		}
	}
}






void AMyProjectCharacter::OnDestroyWall()
{
	if (WallSpawnDestroy != nullptr) {
		WallSpawnDestroy->Destr();
	}
	OnBuildSystem();
}

void AMyProjectCharacter::OnDestroyRoof()
{
	if (RoofSpawnDestroy != nullptr) {
		RoofSpawnDestroy->Destr();

	}
	OnBuildSystem();
}

void AMyProjectCharacter::OnDestroyFloor()
{
	if (FloorSpawnDestroy != nullptr) {
		FloorSpawnDestroy->Destr();
	}
	OnBuildSystem();
}

void AMyProjectCharacter::OnDestroyDoor()
{
	if (DoorSpawnDestroy != nullptr) {
		DoorSpawnDestroy->Destr();
	}
	OnBuildSystem();
}

void AMyProjectCharacter::OnDestroyDoorFrame()
{
	if (DoorFrameSpawnDestroy != nullptr) {
		DoorFrameSpawnDestroy->Destr();
	}
	OnBuildSystem();
}

void AMyProjectCharacter::AddToInventoryByCraft(FString Name, int HowMuchAdd, UTexture2D* Icon)
{
	PickItemName.Add(Name);
	PickItemNum.Add(HowMuchAdd);
	Iconezzz.Add(Icon);

}
