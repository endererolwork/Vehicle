#include "VehiclePhysicsComponent.h"
#include "GameFramework/Actor.h"

UVehiclePhysicsComponent::UVehiclePhysicsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UVehiclePhysicsComponent::BeginPlay()
{
    Super::BeginPlay();
    RootPhysics = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
}

void UVehiclePhysicsComponent::SetInput_Implementation(
    float Throttle, float Steering, bool bBrake)
{
    CurrentInput.Throttle = Throttle;
    CurrentInput.Steering = Steering;
    CurrentInput.bBrake   = bBrake;
}

void UVehiclePhysicsComponent::TickComponent(float DeltaTime,
    ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!RootPhysics || !GetOwner()->HasAuthority()) return;

    ApplyThrottle(DeltaTime);
    ApplySteering(DeltaTime);
    ApplySuspension();
}

void UVehiclePhysicsComponent::ApplyThrottle(float DeltaTime)
{
    if (FMath::IsNearlyZero(CurrentInput.Throttle)) return;

    // bAccelChange=true: physics engine integrates with its own dt, don't multiply DeltaTime here
    const FVector ForwardForce = GetOwner()->GetActorForwardVector()
        * Config.MaxTorque * CurrentInput.Throttle;

    RootPhysics->AddForce(ForwardForce, NAME_None, true);
}

void UVehiclePhysicsComponent::ApplySteering(float DeltaTime)
{
    if (FMath::IsNearlyZero(CurrentInput.Steering)) return;

    // Only steer when moving — prevents spinning in place
    const float Speed = RootPhysics->GetPhysicsLinearVelocity().Size();
    if (Speed < 50.f) return;

    // 200 deg/s² angular accel — with AngularDamping=8, terminal rate ≈ 25 deg/s (full turn ~14s)
    const FVector TorqueVector = FVector(0.f, 0.f, 200.f * CurrentInput.Steering);
    RootPhysics->AddTorqueInDegrees(TorqueVector, NAME_None, true);
}

void UVehiclePhysicsComponent::ApplySuspension()
{
    // Raycast tabanlı süspansiyon — sonraki adımda detaylanacak
}
