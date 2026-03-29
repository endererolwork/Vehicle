#include "RocketLauncher.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

void URocketLauncher::Initialize(UWorld* InWorld, AActor* InOwner)
{
	WorldContext = InWorld;
	OwnerActor = InOwner;
}

void URocketLauncher::Fire_Implementation(const FFireParams& Params)
{
	if (!WorldContext || !OwnerActor || !CanFire_Implementation())
	{
		return;
	}

	// Spawn rocket projectile
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = Cast<APawn>(OwnerActor);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FRotator SpawnRotation = Params.Direction.Rotation();
	
	if (RocketClass)
	{
		WorldContext->SpawnActor<AProjectile>(RocketClass, Params.Origin, SpawnRotation, SpawnParams);
	}

	LastFireTime = WorldContext->GetTimeSeconds();
}

bool URocketLauncher::CanFire_Implementation() const
{
	if (!WorldContext)
	{
		return false;
	}

	const float CurrentTime = WorldContext->GetTimeSeconds();
	return (CurrentTime - LastFireTime) >= FireCooldown;
}

float URocketLauncher::GetCooldown_Implementation() const
{
	return FireCooldown;
}
