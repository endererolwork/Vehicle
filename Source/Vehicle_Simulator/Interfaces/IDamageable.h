#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageable : public UInterface
{
    GENERATED_BODY()
};

class VEHICLE_SIMULATOR_API IDamageable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "Combat")
    void TakeDamage(float Amount, AActor* DamageCauser);

    UFUNCTION(BlueprintNativeEvent, Category = "Combat")
    bool IsAlive() const;

    UFUNCTION(BlueprintNativeEvent, Category = "Combat")
    float GetHealth() const;
};
