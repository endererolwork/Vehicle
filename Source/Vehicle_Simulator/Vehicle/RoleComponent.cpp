#include "RoleComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"

URoleComponent::URoleComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void URoleComponent::BeginPlay()
{
    Super::BeginPlay();
}

void URoleComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(URoleComponent, CurrentRole);
}

void URoleComponent::SwitchRole(
    APlayerController* PlayerController, EVehicleRole NewRole)
{
    if (!PlayerController) return;

    CurrentRole = NewRole;

    switch (NewRole)
    {
        case EVehicleRole::Driver:
            ApplyDriverSetup(PlayerController);
            break;
        case EVehicleRole::Gunner:
            ApplyGunnerSetup(PlayerController);
            break;
        default:
            break;
    }
}

void URoleComponent::ApplyDriverSetup(APlayerController* PC)
{
    // Input Mapping Context swap — PlayerController'da detaylanacak
}

void URoleComponent::ApplyGunnerSetup(APlayerController* PC)
{
    // Input Mapping Context swap — PlayerController'da detaylanacak
}
