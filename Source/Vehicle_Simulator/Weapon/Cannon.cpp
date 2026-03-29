#include "Cannon.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

void UCannon::Initialize(UWorld* InWorld, AActor* InOwner)
{
	WorldContext = InWorld;
	OwnerActor = InOwner;
}

void UCannon::Fire_Implementation(const FFireParams& Params)
{
	if (!WorldContext || !OwnerActor || !CanFire_Implementation())
	{
		return;
	}

	// Spawn projectile
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = Cast<APawn>(OwnerActor);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FRotator SpawnRotation = Params.Direction.Rotation();
	
	if (CannonShellClass)
	{
		WorldContext->SpawnActor<AProjectile>(CannonShellClass, Params.Origin, SpawnRotation, SpawnParams);
	}

	LastFireTime = WorldContext->GetTimeSeconds();
}

bool UCannon::CanFire_Implementation() const
{
	if (!WorldContext)
	{
		return false;
	}

	const float CurrentTime = WorldContext->GetTimeSeconds();
	return (CurrentTime - LastFireTime) >= FireCooldown;
}

float UCannon::GetCooldown_Implementation() const
{
	return FireCooldown;
}
