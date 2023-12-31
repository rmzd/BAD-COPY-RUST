// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "AIEnemyController.h"
#include "MyProjectCharacter.h"
#include "Wolf.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


// Sets default values
AAICharacter::AAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

    ApproachTriggerCapsule = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    ApproachTriggerCapsule->InitSphereRadius(200.f);
    ApproachTriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
    ApproachTriggerCapsule->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
    ApproachTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnOverlapBegin);

	AIEnemyController = Cast<AAIEnemyController>(GetController());
	if (AIEnemyController != nullptr) {
		AIEnemyController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AAICharacter::OnAIMoveCompleted);
	}
    IsDead = false;
    IsPredator = false;
    IsInDanger = false;
    MiningStage = 0;

    CurrentHealth=MaxHealth;
    CurrentFood = FMath::RandRange(0, 100);
    CurrentSleep = FMath::RandRange(0, 100);

    NextMove();
    CheckIsDangerous();
    GetLessFood();
    GetLessSleep();
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

int AAICharacter::GetMiningStage()
{
    return MiningStage;
}



void AAICharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (!IsDead) {
            if (IsPredator && Cast<AMyProjectCharacter>(OtherActor)) {
                PlayAnim(Bite, 1);
                NextMove();
            }
            
        }
       
    }

}

void AAICharacter::Eat()
{
    if (!IsSleeping && !IsInDanger) {
        GetCharacterMovement()->StopMovementImmediately();
        if (!IsEating) {
            PlayAnim(AnimEat, true);
           
        }
            IsEating = true;
            GetMoreFood();
    }
}

void AAICharacter::Sleep()
{

    if (!IsEating && !IsInDanger) { 
        GetCharacterMovement()->StopMovementImmediately();
        if (!IsSleeping) {
            PlayAnim(GoToRest, false);
            GetWorld()->GetTimerManager().SetTimer(SleepAnimTime, this, &AAICharacter::PlayAnimSleep, 2.f, false);
            
        }
        IsSleeping = true;
        GetMoreSleep();
    }
}

void AAICharacter::Run()
{

    GetCharacterMovement()->MaxWalkSpeed = FMath::RandRange(MinSpeedRun, MaxSpeedRun);
    if (!IsRunnig) {
        PlayAnim(AnimRun, true);
        
    }
    IsRunnig = true;
  
   
    IsEating = IsSleeping = false;
    if (!IsPlayingAnimalSound) {
        if (AnimalSound != nullptr) {
            AudioComponent=UGameplayStatics::SpawnSoundAttached(AnimalSound, GetMesh());
            IsPlayingAnimalSound = 1;
       }
    }
      if (IsPredator) {
        FVector newvec = ((GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation())-GetActorLocation()).Rotation().Vector() * (500) + GetActorLocation();

        FVector newvector = FVector(0.f);
        newvector += FVector(newvec.X + FMath::RandRange(-100, 100), newvec.Y + FMath::RandRange(-100, 100),GetActorLocation().Z);
        AIEnemyController->MoveTo(newvector);
    }
    else {
        FVector newvec = ((GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()) - GetActorLocation()).Rotation().Vector() * (-500) + GetActorLocation();
        FVector newvector = FVector(0.f);

        newvector += FVector(newvec.X + FMath::RandRange(-10, 10), newvec.Y + FMath::RandRange(-10, 10), GetActorLocation().Z);

        AIEnemyController->MoveTo(newvector);
    }


    
}

void AAICharacter::Walk()
{
    if (!IsWalking) {

        if (!IsWalking) {
            
            PlayAnim(AnimWalk, true);
        }
        IsWalking = true;
        GetCharacterMovement()->MaxWalkSpeed = 100.f;
        AIEnemyController->RandomPatrol();
    }
}


//What should do animal
void AAICharacter::NextMove()
{
    if (!IsDead) {
        if (CurrentSleep <= 0) {
            IsSleepy = 1;
        }
        else {
            IsSleepy = 0;
        }
        if (CurrentFood <= 0) {
            IsHungry = 1;
        }
        else {
            IsHungry = 0;
        }
        if (IsInDanger) {
            Run();

        }
        else if (IsHungry) {
            Eat();
        }
        else if (IsSleepy) {
            Sleep();
        }
        else if (FMath::RandRange(0, 1) == 1) {
            Walk();
        }
        else {
            PlayAnim(AnimDefault, true);
        }
    }
}

void AAICharacter::PlayAnim(UAnimSequence* AnimToPlay,bool ShoulRepeat)
{
    GetMesh()->PlayAnimation(AnimToPlay, ShoulRepeat);
}

void AAICharacter::StopAnim()
{
    GetMesh()->Stop();
}

void AAICharacter::PlayAnimSleep()
{
    if (IsSleeping) {
        PlayAnim(AnimSleep, true);
    }
    
}


void AAICharacter::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    IsWalking = false;
    NextMove();
}


void AAICharacter::CheckIsDangerous()
{
    if (!IsDead) {
        if (IsInDanger) {
            GetWorld()->GetTimerManager().SetTimer(CheckDangerous, this, &AAICharacter::CheckIsDangerous, .5f, false);
        }
        else {
            GetWorld()->GetTimerManager().SetTimer(CheckDangerous, this, &AAICharacter::CheckIsDangerous, 0.1f, false);

        }
        float distanceToPlayer = (GetActorLocation() - (GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation())).Size();
        if (distanceToPlayer < 1500) {
            IsInDanger = true;
            // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentSleep %f"), distanceToPlayer));
            NextMove();
        }
        if (distanceToPlayer > 5500) {
            IsInDanger = false;
            IsRunnig = false;


        }
    }
}

void AAICharacter::GetLessSleep()
{
   // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentSleep %i"), CurrentSleep));
    if (!IsDead && IsSleeping == false && CurrentSleep>0) {
        CurrentSleep--;
        GetWorld()->GetTimerManager().SetTimer(TimerSleep, this, &AAICharacter::GetLessSleep, TimerTime, false);

        if (CurrentSleep <= 0) {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NextMove"));

            NextMove();
        }
    }
}

void AAICharacter::GetLessFood()
{
   // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentFood %i"), CurrentFood));
    if (!IsDead && IsEating == false && CurrentFood > 0) {
        CurrentFood--;
       
        GetWorld()->GetTimerManager().SetTimer(Timer, this, &AAICharacter::GetLessFood, TimerTime, false);

        if (CurrentFood <= 0) {
            
            NextMove();
            
        }
    }
}

void AAICharacter::GetMoreFood()
{
  // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentFood %i"), CurrentFood));
    if (!IsDead && IsEating == true && CurrentFood < 100) {
        CurrentFood++;
        GetWorld()->GetTimerManager().SetTimer(Timer, this, &AAICharacter::GetMoreFood, TimerTime, false);
        if (CurrentFood == MaxFood) {
            IsEating = false;
            GetWorld()->GetTimerManager().SetTimer(Timer, this, &AAICharacter::GetMoreFood, TimerTime, false);
           
            //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("GetMoreFood!"));
        }
    }
    else {
        NextMove();
        IsHungry = false;

        GetWorld()->GetTimerManager().SetTimer(Timer, this, &AAICharacter::GetLessFood, TimerTime, false);
    }
}

void AAICharacter::GetMoreSleep()
{
   // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentSleep %i"), CurrentSleep));
    if (!IsDead && IsSleeping == true && CurrentSleep < 100) {
        CurrentSleep++;
        GetWorld()->GetTimerManager().SetTimer(TimerSleep, this, &AAICharacter::GetMoreSleep, TimerTime, false);
        if (CurrentSleep == MaxSleep) {
            IsSleeping = false;
            
            GetWorld()->GetTimerManager().SetTimer(TimerSleep, this, &AAICharacter::GetMoreSleep, TimerTime, false);
          //  GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("GetMoreSleep!"));
        }
    }
    else {
        NextMove();
        IsSleeping = false;

        GetWorld()->GetTimerManager().SetTimer(TimerSleep, this, &AAICharacter::GetLessSleep, TimerTime, false);
    }
}

void AAICharacter::GetMoreHealth()
{
    /*if (CurrentSleep == MaxSleep / 2 && CurrentFood == MaxFood / 2) {
        CurrentHealth++;
    }*/
}

void AAICharacter::DamageHealth(float GetDamage)
{
    if (Health > 0) {
        Health -= GetDamage;
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Health %f"), Health));
        
    }
    if (Health<0) {
        if (!IsDead) {
            Dead();

        }
    }
}

void AAICharacter::Dead()
{
    IsDead = true;
    
    if (DeadSound != nullptr) {
        UGameplayStatics::SpawnSoundAttached(DeadSound, GetMesh());

    }
    if (AudioComponent != nullptr) {
       AudioComponent->Stop();
    }

    GetMovementComponent()->StopMovementImmediately();
   // DetachFromControllerPendingDestroy();
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
    GetMesh()->SetAllBodiesSimulatePhysics(true);
    GetMesh()->SetSimulatePhysics(true);

}

void AAICharacter::IncrementMiningStage()
{
    MiningStage++;
    if (MiningStage > 2) {
        Destroy();
    }
}


