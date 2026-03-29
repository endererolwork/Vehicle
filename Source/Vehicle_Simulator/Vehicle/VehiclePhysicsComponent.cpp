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

    const FVector ForwardForce = GetOwner()->GetActorForwardVector()
        * Config.MaxTorque * CurrentInput.Throttle * DeltaTime;

    RootPhysics->AddForce(ForwardForce, NAME_None, true);
}

void UVehiclePhysicsComponent::ApplySteering(float DeltaTime)
{
    if (FMath::IsNearlyZero(CurrentInput.Steering)) return;

    const FVector TorqueVector = FVector(0.f, 0.f,
        Config.MaxTorque * 0.1f * CurrentInput.Steering * DeltaTime);

    RootPhysics->AddTorqueInDegrees(TorqueVector, NAME_None, true);
}

void UVehiclePhysicsComponent::ApplySuspension()
{
    // Raycast tabanlı süspansiyon — sonraki adımda detaylanacak
}
