#include "TurretComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

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

void UTurretComponent::Initialize(
    UStaticMeshComponent* InTurretMesh, UStaticMeshComponent* InBarrelMesh)
{
    TurretMesh = InTurretMesh;
    BarrelMesh = InBarrelMesh;
}

void UTurretComponent::ApplyMeshRotation()
{
    if (TurretMesh)
    {
        TurretMesh->SetRelativeRotation(FRotator(0.f, TurretRotation.Yaw, 0.f));
    }
    if (BarrelMesh)
    {
        BarrelMesh->SetRelativeRotation(FRotator(TurretRotation.Pitch, 0.f, 0.f));
    }
}

void UTurretComponent::RotateTurret(
    float YawInput, float PitchInput, float DeltaTime)
{
    FRotator NewRotation = TurretRotation;
    NewRotation.Yaw   += YawInput   * TurretRotationSpeed * DeltaTime;
    NewRotation.Pitch  = FMath::Clamp(
        NewRotation.Pitch + PitchInput * TurretRotationSpeed * DeltaTime,
        BarrelPitchMin, BarrelPitchMax);

    // Client-side prediction: hemen uygula, sunucuya gönder
    TurretRotation = NewRotation;
    ApplyMeshRotation();

    Server_RotateTurret(NewRotation);
}

void UTurretComponent::Server_RotateTurret_Implementation(FRotator NewRotation)
{
    TurretRotation = NewRotation;
    ApplyMeshRotation(); // sunucu tarafında mesh güncelle
}

bool UTurretComponent::Server_RotateTurret_Validate(FRotator NewRotation)
{
    return true;
}

void UTurretComponent::OnRep_TurretRotation()
{
    ApplyMeshRotation();
}
