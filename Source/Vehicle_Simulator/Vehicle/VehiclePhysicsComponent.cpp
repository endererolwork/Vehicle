#include "VehiclePhysicsComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

UVehiclePhysicsComponent::UVehiclePhysicsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UVehiclePhysicsComponent::BeginPlay()
{
    Super::BeginPlay();
    RootPhysics = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    UE_LOG(LogTemp, Warning, TEXT("PhysicsComponent BeginPlay — RootPhysics: %s"),
        RootPhysics ? TEXT("OK") : TEXT("NULL"));
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

    if (!RootPhysics) return;

    SmoothedSteering = FMath::FInterpTo(SmoothedSteering, CurrentInput.Steering, DeltaTime, 5.f);

    GEngine->AddOnScreenDebugMessage(3, 0.1f, FColor::Yellow,
        FString::Printf(TEXT("PhysicsTick | Throttle=%.2f Steering=%.2f | Speed=%.1f"),
            CurrentInput.Throttle, CurrentInput.Steering, CurrentSpeed));

    ApplyThrottle(DeltaTime);
    ApplySteering(DeltaTime);
}

void UVehiclePhysicsComponent::ApplyThrottle(float DeltaTime)
{
    // W > 0 = ileri, S < 0 = geri, 0 = dur
    float TargetSpeed = 0.f;
    if (CurrentInput.Throttle > 0.f)
        TargetSpeed = Config.MaxForwardSpeed;
    else if (CurrentInput.Throttle < 0.f)
        TargetSpeed = -Config.MaxReverseSpeed;

    const float Rate = FMath::IsNearlyZero(TargetSpeed) ? Config.DecelerationRate : Config.AccelerationRate;
    CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, Rate);

    // Z hızını koru (yerçekimi çalışsın), sadece ileri/geri uygula
    const FVector Forward     = GetOwner()->GetActorForwardVector();
    const FVector CurrentVel  = RootPhysics->GetPhysicsLinearVelocity();
    RootPhysics->SetPhysicsLinearVelocity(
        FVector(Forward.X, Forward.Y, 0.f).GetSafeNormal() * CurrentSpeed
        + FVector(0.f, 0.f, CurrentVel.Z)
    );
}

void UVehiclePhysicsComponent::ApplySteering(float DeltaTime)
{
    // Fizik motorunun istem dışı döndürmesini önle
    RootPhysics->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

    if (FMath::IsNearlyZero(SmoothedSteering)) return;

    // A = -1 = sol, D = +1 = sağ
    const float YawDelta = Config.MaxYawRate * SmoothedSteering * DeltaTime;
    GetOwner()->AddActorLocalRotation(
        FRotator(0.f, YawDelta, 0.f),
        false,
        nullptr,
        ETeleportType::TeleportPhysics
    );
}
