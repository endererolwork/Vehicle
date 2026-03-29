#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurretComponent.generated.h"

UCLASS(ClassGroup=(Vehicle), meta=(BlueprintSpawnableComponent))
class VEHICLE_SIMULATOR_API UTurretComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTurretComponent();

    UFUNCTION(BlueprintCallable, Category = "Turret")
    void RotateTurret(float YawInput, float PitchInput, float DeltaTime);

    UFUNCTION(BlueprintPure, Category = "Turret")
    FRotator GetTurretRotation() const { return TurretRotation; }

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_RotateTurret(FRotator NewRotation);

    UFUNCTION()
    void OnRep_TurretRotation();

    UPROPERTY(ReplicatedUsing = OnRep_TurretRotation)
    FRotator TurretRotation = FRotator::ZeroRotator;

    UPROPERTY(EditDefaultsOnly, Category = "Turret")
    float TurretRotationSpeed = 60.f;

    UPROPERTY(EditDefaultsOnly, Category = "Turret")
    float BarrelPitchMin = -10.f;

    UPROPERTY(EditDefaultsOnly, Category = "Turret")
    float BarrelPitchMax = 20.f;

    UPROPERTY()
    TObjectPtr<UStaticMeshComponent> TurretMesh;

    UPROPERTY()
    TObjectPtr<UStaticMeshComponent> BarrelMesh;
};
