#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Interfaces/IWeapon.h"
#include "MachineGun.generated.h"

UCLASS(Blueprintable, BlueprintType)
class VEHICLE_SIMULATOR_API UMachineGun : public UObject, public IWeapon
{
	GENERATED_BODY()

public:
	// IWeapon interface
	virtual void Fire_Implementation(const FFireParams& Params) override;
	virtual bool CanFire_Implementation() const override;
	virtual float GetCooldown_Implementation() const override;

	void Initialize(UWorld* InWorld, AActor* InOwner);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "MachineGun")
	float FireCooldown = 0.1f; // Hızlı ateş

	UPROPERTY(EditDefaultsOnly, Category = "MachineGun")
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "MachineGun")
	float MaxRange = 5000.0f;

private:
	void PerformHitscan(const FFireParams& Params);

	float LastFireTime = 0.0f;
	
	UPROPERTY()
	TObjectPtr<UWorld> WorldContext;
	
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;
};
