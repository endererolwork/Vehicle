#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRoleSwappable.generated.h"

UENUM(BlueprintType)
enum class EVehicleRole : uint8
{
    None    UMETA(DisplayName = "None"),
    Driver  UMETA(DisplayName = "Driver"),
    Gunner  UMETA(DisplayName = "Gunner")
};

UINTERFACE(MinimalAPI, Blueprintable)
class URoleSwappable : public UInterface
{
    GENERATED_BODY()
};

class VEHICLE_SIMULATOR_API IRoleSwappable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "Role")
    void OnRoleChanged(EVehicleRole NewRole);

    UFUNCTION(BlueprintNativeEvent, Category = "Role")
    EVehicleRole GetCurrentRole() const;
};
