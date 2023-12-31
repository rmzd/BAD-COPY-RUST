// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "AICharacter.h"
#include "BuildSystem.h"
#include "Kismet/GameplayStatics.h"


AMyProjectProjectile::AMyProjectProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMyProjectProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 20000.f;
	ProjectileMovement->MaxSpeed = 20000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	ItemIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Script/Engine.Texture2D'/Game/Texture2D/ammo_rifle.ammo_rifle'")));
	NumItemInOne = 100;

}

void AMyProjectProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

		if (FireSound != nullptr) {
			if (Cast<ABuildSystem>(OtherActor)) {
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
			
		}
		if (OtherComp->IsSimulatingPhysics()) {
			OtherComp->AddImpulseAtLocation(GetVelocity() * 0.1f, GetActorLocation());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnHit"));
		}


		
	}
	if (Cast<AAICharacter>(OtherActor)) {
		{
			Cast<AAICharacter>(OtherActor)->DamageHealth(30);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cast"));

		}
	}
	if (Cast<ABuildSystem>(OtherActor)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ABuildSystem"));
		Cast<ABuildSystem>(OtherActor)->Damage(100);
	}
	Destroy();
}