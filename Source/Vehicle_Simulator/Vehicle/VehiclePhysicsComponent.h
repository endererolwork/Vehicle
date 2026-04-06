#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interfaces/IControllable.h"
#include "../VehicleTypes.h"
#include "VehiclePhysicsComponent.generated.h"

UCLASS(ClassGroup=(Vehicle), meta=(BlueprintSpawnableComponent))
class VEHICLE_SIMULATOR_API UVehiclePhysicsComponent : public UActorComponent,
    public IControllable
{
    GENERATED_BODY()

public:
    UVehiclePhysicsComponent();

    // IControllable
    virtual void SetInput_Implementation(float Throttle, float Steering, bool bBrake) override;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void BeginPlay() override;

private:
    void ApplyThrottle(float DeltaTime);
    void ApplySteering(float DeltaTime);
    void ApplySuspension();

    UPROPERTY(EditAnywhere, Category = "Physics")
    FVehiclePhysicsConfig Config;

    FVehicleInput CurrentInput;

    float SmoothedSteering = 0.f;
    float CurrentSpeed = 0.f;

    UPROPERTY()
    TObjectPtr<UPrimitiveComponent> RootPhysics;
};
