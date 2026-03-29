#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interfaces/IRoleSwappable.h"
#include "RoleComponent.generated.h"

UCLASS(ClassGroup=(Vehicle), meta=(BlueprintSpawnableComponent))
class VEHICLE_SIMULATOR_API URoleComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URoleComponent();

    UFUNCTION(BlueprintCallable, Category = "Role")
    void SwitchRole(APlayerController* PlayerController, EVehicleRole NewRole);

    UFUNCTION(BlueprintPure, Category = "Role")
    EVehicleRole GetCurrentRole() const { return CurrentRole; }

protected:
    virtual void BeginPlay() override;

private:
    void ApplyDriverSetup(APlayerController* PC);
    void ApplyGunnerSetup(APlayerController* PC);

    UPROPERTY(Replicated)
    EVehicleRole CurrentRole = EVehicleRole::None;

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
