#include "MachineGun.h"
#include "../Interfaces/IDamageable.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

void UMachineGun::Initialize(UWorld* InWorld, AActor* InOwner)
{
	WorldContext = InWorld;
	OwnerActor = InOwner;
}

void UMachineGun::Fire_Implementation(const FFireParams& Params)
{
	if (!WorldContext || !OwnerActor || !CanFire_Implementation())
	{
		return;
	}

	PerformHitscan(Params);
	LastFireTime = WorldContext->GetTimeSeconds();
}

bool UMachineGun::CanFire_Implementation() const
{
	if (!WorldContext)
	{
		return false;
	}

	const float CurrentTime = WorldContext->GetTimeSeconds();
	return (CurrentTime - LastFireTime) >= FireCooldown;
}

float UMachineGun::GetCooldown_Implementation() const
{
	return FireCooldown;
}

void UMachineGun::PerformHitscan(const FFireParams& Params)
{
	const FVector Start = Params.Origin;
	const FVector End = Start + (Params.Direction * MaxRange);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor);

	if (WorldContext->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
	{
		// Hit something
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->Implements<UDamageable>())
		{
			IDamageable::Execute_TakeDamage(HitActor, Damage, OwnerActor);
		}

		// Debug line
		DrawDebugLine(WorldContext, Start, HitResult.Location, FColor::Yellow, false, 0.5f, 0, 1.0f);
	}
	else
	{
		// No hit
		DrawDebugLine(WorldContext, Start, End, FColor::Red, false, 0.5f, 0, 1.0f);
	}
}
