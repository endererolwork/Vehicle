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

    SmoothedThrottle = FMath::FInterpTo(SmoothedThrottle, CurrentInput.Throttle, DeltaTime, 3.f);
    SmoothedSteering = FMath::FInterpTo(SmoothedSteering, CurrentInput.Steering, DeltaTime, 3.f);

    ApplyThrottle(DeltaTime);
    ApplySteering(DeltaTime);
    ApplySuspension();
}

void UVehiclePhysicsComponent::ApplyThrottle(float DeltaTime)
{
    if (FMath::IsNearlyZero(SmoothedThrottle)) return;

    const FVector ForwardForce = GetOwner()->GetActorForwardVector()
        * Config.MaxTorque * SmoothedThrottle;

    RootPhysics->AddForce(ForwardForce, NAME_None, true);
}

void UVehiclePhysicsComponent::ApplySteering(float DeltaTime)
{
    // Physics angular velocity'yi sıfırla — fizik motorunun istem dışı döndürmesini önler
    RootPhysics->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

    if (FMath::IsNearlyZero(SmoothedSteering)) return;

    // Direkt rotation: A=-MaxYawRate deg/s, D=+MaxYawRate deg/s
    const float YawDelta = Config.MaxYawRate * SmoothedSteering * DeltaTime;
    GetOwner()->AddActorLocalRotation(
        FRotator(0.f, YawDelta, 0.f),
        false,
        nullptr,
        ETeleportType::TeleportPhysics
    );
}

void UVehiclePhysicsComponent::ApplySuspension()
{
    // Raycast tabanlı süspansiyon — sonraki adımda detaylanacak
}
