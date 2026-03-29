#include "ExplosiveShell.h"
#include "../Interfaces/IDamageable.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AExplosiveShell::AExplosiveShell()
{
	Damage = 50.0f; // Direct hit damage
	InitialSpeed = 1500.0f;
	MaxSpeed = 1500.0f;
}

void AExplosiveShell::OnImpact(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority()) return;

	const FVector ExplosionLocation = GetActorLocation();

	// Apply radial damage
	ApplyRadialDamage(ExplosionLocation);

	// Debug sphere
	DrawDebugSphere(GetWorld(), ExplosionLocation, ExplosionRadius, 12, FColor::Orange, false, 2.0f);

	// Destroy shell
	Destroy();
}

void AExplosiveShell::ApplyRadialDamage(const FVector& ExplosionLocation)
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(ExplosionRadius);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		ExplosionLocation,
		FQuat::Identity,
		ECC_Pawn,
		CollisionShape,
		QueryParams
	);

	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* HitActor = Result.GetActor();
		if (HitActor && HitActor->Implements<UDamageable>())
		{
			// Calculate distance-based damage
			const float Distance = FVector::Dist(ExplosionLocation, HitActor->GetActorLocation());
			const float DamageFalloff = 1.0f - (Distance / ExplosionRadius);
			const float FinalDamage = ExplosionDamage * FMath::Max(0.0f, DamageFalloff);

			IDamageable::Execute_TakeDamage(HitActor, FinalDamage, GetOwner());
		}
	}
}
