#include "TurretComponent.h"
#include "Net/UnrealNetwork.h"

UTurretComponent::UTurretComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTurretComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UTurretComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UTurretComponent, TurretRotation);
}

void UTurretComponent::RotateTurret(
    float YawInput, float PitchInput, float DeltaTime)
{
    FRotator NewRotation = TurretRotation;
    NewRotation.Yaw   += YawInput   * TurretRotationSpeed * DeltaTime;
    NewRotation.Pitch  = FMath::Clamp(
        NewRotation.Pitch + PitchInput * TurretRotationSpeed * DeltaTime,
        BarrelPitchMin, BarrelPitchMax);

    Server_RotateTurret(NewRotation);
}

void UTurretComponent::Server_RotateTurret_Implementation(FRotator NewRotation)
{
    TurretRotation = NewRotation;
}

bool UTurretComponent::Server_RotateTurret_Validate(FRotator NewRotation)
{
    return true;
}

void UTurretComponent::OnRep_TurretRotation()
{
    if (TurretMesh)
    {
        TurretMesh->SetRelativeRotation(
            FRotator(0.f, TurretRotation.Yaw, 0.f));
    }
    if (BarrelMesh)
    {
        BarrelMesh->SetRelativeRotation(
            FRotator(TurretRotation.Pitch, 0.f, 0.f));
    }
}
