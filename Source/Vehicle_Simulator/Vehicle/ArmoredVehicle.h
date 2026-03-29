#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Interfaces/IDamageable.h"
#include "../Interfaces/IControllable.h"
#include "../Interfaces/IRoleSwappable.h"
#include "../VehicleTypes.h"
#include "ArmoredVehicle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoleChanged, EVehicleRole, NewRole);

class UVehiclePhysicsComponent;
class UTurretComponent;
class UWeaponComponent;
class URoleComponent;
class USkeletalMeshComponent;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class VEHICLE_SIMULATOR_API AArmoredVehicle : public APawn,
    public IDamageable,
    public IControllable,
    public IRoleSwappable
{
    GENERATED_BODY()

public:
    AArmoredVehicle();

    // IDamageable
    virtual void TakeDamage_Implementation(float Amount, AActor* DamageCauser) override;
    virtual bool IsAlive_Implementation() const override;
    virtual float GetHealth_Implementation() const override;

    // IControllable
    virtual void SetInput_Implementation(float Throttle, float Steering, bool bBrake) override;

    // IRoleSwappable
    virtual void OnRoleChanged_Implementation(EVehicleRole NewRole) override;
    virtual EVehicleRole GetCurrentRole_Implementation() const override;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Vehicle")
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Vehicle")
    FOnRoleChanged OnVehicleRoleChanged;

    // Getters
    UFUNCTION(BlueprintPure, Category = "Vehicle")
    UVehiclePhysicsComponent* GetPhysicsComponent() const { return PhysicsComponent; }

    UFUNCTION(BlueprintPure, Category = "Vehicle")
    UTurretComponent* GetTurretComponent() const { return TurretComponent; }

    UFUNCTION(BlueprintPure, Category = "Vehicle")
    UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Server RPCs
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_SetInput(float Throttle, float Steering, bool bBrake);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_SwitchRole(EVehicleRole NewRole);

    // OnRep callbacks
    UFUNCTION()
    void OnRep_Health();

    UFUNCTION()
    void OnRep_CurrentRole();

private:
    // Components — SRP: her biri tek sorumlu
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
              meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USkeletalMeshComponent> VehicleMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
              meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UBoxComponent> CollisionBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
              meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
              meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> ThirdPersonCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
              meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> SightCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
              meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UVehiclePhysicsComponent> PhysicsComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
              meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UTurretComponent> TurretComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
              meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UWeaponComponent> WeaponComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
              meta = (AllowPrivateAccess = "true"))
    TObjectPtr<URoleComponent> RoleComponent;

    // Replicated state
    UPROPERTY(ReplicatedUsing = OnRep_Health)
    float Health = 100.f;

    UPROPERTY(EditDefaultsOnly, Category = "Vehicle")
    float MaxHealth = 100.f;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentRole)
    EVehicleRole CurrentRole = EVehicleRole::None;

    // Input state (local)
    FVehicleInput CurrentInput;
};
