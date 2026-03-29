#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Interfaces/IWeapon.h"
#include "Cannon.generated.h"

class AProjectile;

UCLASS(Blueprintable, BlueprintType)
class VEHICLE_SIMULATOR_API UCannon : public UObject, public IWeapon
{
	GENERATED_BODY()

public:
	// IWeapon interface
	virtual void Fire_Implementation(const FFireParams& Params) override;
	virtual bool CanFire_Implementation() const override;
	virtual float GetCooldown_Implementation() const override;

	void Initialize(UWorld* InWorld, AActor* InOwner);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Cannon")
	TSubclassOf<AProjectile> CannonShellClass;

	UPROPERTY(EditDefaultsOnly, Category = "Cannon")
	float FireCooldown = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Cannon")
	float MuzzleVelocity = 2000.0f;

private:
	float LastFireTime = 0.0f;
	
	UPROPERTY()
	TObjectPtr<UWorld> WorldContext;
	
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;
};
