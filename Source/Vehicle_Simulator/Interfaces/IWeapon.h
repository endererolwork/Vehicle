#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../VehicleTypes.h"
#include "IWeapon.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UWeapon : public UInterface
{
    GENERATED_BODY()
};

class VEHICLE_SIMULATOR_API IWeapon
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
    void Fire(const FFireParams& Params);

    UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
    bool CanFire() const;

    UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
    float GetCooldown() const;
};
