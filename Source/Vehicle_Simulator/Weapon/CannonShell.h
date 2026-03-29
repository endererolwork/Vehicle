#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "CannonShell.generated.h"

UCLASS()
class VEHICLE_SIMULATOR_API ACannonShell : public AProjectile
{
	GENERATED_BODY()

public:
	ACannonShell();

protected:
	virtual void OnImpact(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit) override;

	UPROPERTY(EditDefaultsOnly, Category = "CannonShell")
	float ArmorPenetration = 100.0f;
};
