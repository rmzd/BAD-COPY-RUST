// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UCLASS()
class MYPROJECT_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

public:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* ApproachTriggerCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* AnimalSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* DeadSound;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations")
	UAnimSequence* AnimSleep;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations")
	UAnimSequence* AnimEat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations")
	UAnimSequence* AnimRun;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations")
	UAnimSequence* AnimWalk;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations")
	UAnimSequence* AnimDefault;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations")
	UAnimSequence* GoToRest;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations")
	UAnimSequence* ReturnFromRest;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations")
	UAnimSequence* Bite;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(BlueprintReadWrite)
	bool CanDealDamage;

	UAudioComponent* AudioComponent;

	class AEnemy_AICharacter* PlayerREF;

	class AAIEnemyController* AIEnemyController;

	FTimerHandle Timer;
	FTimerHandle TimerSleep;
	FTimerHandle CheckDangerous;
	FTimerHandle RandFloatTimer;
	FTimerHandle SleepAnimTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	bool IsPlayingAnimalSound;

	float TimerTime = 0.2f;

	float Health;

	float MaxSpeedRun;

	float MinSpeedRun;

	int RandFloat;

	int RandFloatSum;

	int CurrentHealth;

	int MaxHealth;

	int CurrentFood;

	int MaxFood;

	int CurrentSleep;

	int MaxSleep;

	bool IsDead;

	bool IsEating;

	bool IsSleeping;

	bool IsHealthing;

	bool IsHungry;

	bool IsSleepy;

	bool IsInDanger;

	bool IsRunnig;

	bool IsWalking;

	bool IsPredator;

	int MiningStage;

	bool PlayerDetected;

	bool CanAttackPlayer;
	public:
	void CheckIsDangerous();

	void GetLessSleep();

	void GetLessFood();

	void GetMoreFood();

	void GetMoreSleep();

	void GetMoreHealth();

	void Dead();

	void Eat();

	void Sleep();

	void Run();

	void Walk();

	void NextMove();

	void PlayAnim(UAnimSequence* AnimToPlay, bool ShoulRepeat);

	void StopAnim();

	void PlayAnimSleep();

	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

	int GetMiningStage();

	void IncrementMiningStage();

	void DamageHealth(float GetDamage);
};


