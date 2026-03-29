#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Interfaces/IWeapon.h"
#include "RocketLauncher.generated.h"

class AProjectile;

UCLASS(Blueprintable, BlueprintType)
class VEHICLE_SIMULATOR_API URocketLauncher : public UObject, public IWeapon
{
	GENERATED_BODY()

public:
	// IWeapon interface
	virtual void Fire_Implementation(const FFireParams& Params) override;
	virtual bool CanFire_Implementation() const override;
	virtual float GetCooldown_Implementation() const override;

	void Initialize(UWorld* InWorld, AActor* InOwner);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "RocketLauncher")
	TSubclassOf<AProjectile> RocketClass;

	UPROPERTY(EditDefaultsOnly, Category = "RocketLauncher")
	float FireCooldown = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "RocketLauncher")
	float MuzzleVelocity = 3000.0f;

private:
	float LastFireTime = 0.0f;
	
	UPROPERTY()
	TObjectPtr<UWorld> WorldContext;
	
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;
};
