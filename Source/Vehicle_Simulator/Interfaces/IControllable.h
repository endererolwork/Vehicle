#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IControllable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UControllable : public UInterface
{
    GENERATED_BODY()
};

class VEHICLE_SIMULATOR_API IControllable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "Control")
    void SetInput(float Throttle, float Steering, bool bBrake);
};
