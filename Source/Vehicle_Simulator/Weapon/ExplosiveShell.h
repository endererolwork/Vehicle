#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ExplosiveShell.generated.h"

UCLASS()
class VEHICLE_SIMULATOR_API AExplosiveShell : public AProjectile
{
	GENERATED_BODY()

public:
	AExplosiveShell();

protected:
	virtual void OnImpact(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit) override;

	UPROPERTY(EditDefaultsOnly, Category = "Explosive")
	float ExplosionRadius = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Explosive")
	float ExplosionDamage = 100.0f;

private:
	void ApplyRadialDamage(const FVector& ExplosionLocation);
};
