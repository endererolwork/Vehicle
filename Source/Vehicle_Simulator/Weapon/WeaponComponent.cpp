#include "WeaponComponent.h"
#include "Cannon.h"
#include "MachineGun.h"
#include "RocketLauncher.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    // Blueprint'te DefaultWeaponClass set edildiyse onu, yoksa Cannon fallback
    TSubclassOf<UObject> WeaponToUse = DefaultWeaponClass
        ? DefaultWeaponClass
        : TSubclassOf<UObject>(UCannon::StaticClass());
    SetActiveWeaponByClass(WeaponToUse);
}

void UWeaponComponent::SetActiveWeaponByClass(TSubclassOf<UObject> WeaponClass)
{
    if (!WeaponClass) return;

    // Create weapon instance
    UObject* WeaponObject = NewObject<UObject>(this, WeaponClass);

    // Verify it implements IWeapon
    if (WeaponObject && WeaponObject->Implements<UWeapon>())
    {
        ActiveWeapon.SetObject(WeaponObject);
        ActiveWeapon.SetInterface(Cast<IWeapon>(WeaponObject));

        // Initialize weapon (if it has Initialize method)
        // This is specific to our weapon implementations
        if (UCannon* Cannon = Cast<UCannon>(WeaponObject))
        {
            Cannon->Initialize(GetWorld(), GetOwner());
        }
        else if (UMachineGun* MG = Cast<UMachineGun>(WeaponObject))
        {
            MG->Initialize(GetWorld(), GetOwner());
        }
        else if (URocketLauncher* RL = Cast<URocketLauncher>(WeaponObject))
        {
            RL->Initialize(GetWorld(), GetOwner());
        }
    }
}

void UWeaponComponent::RequestFire(const FFireParams& Params)
{
    if (!ActiveWeapon.GetObject()) return;

    // DIP: Call through interface - we don't know the concrete type
    if (IWeapon::Execute_CanFire(ActiveWeapon.GetObject()))
    {
        Server_Fire(Params);
    }
}

void UWeaponComponent::Server_Fire_Implementation(const FFireParams& Params)
{
    if (!GetOwner()->HasAuthority() || !ActiveWeapon.GetObject()) return;

    // DIP: Fire through interface
    IWeapon::Execute_Fire(ActiveWeapon.GetObject(), Params);

    Multicast_FireEffects(Params.Origin, Params.Direction);
}

bool UWeaponComponent::Server_Fire_Validate(const FFireParams& Params)
{
    return Params.Direction.IsNormalized();
}

void UWeaponComponent::Multicast_FireEffects_Implementation(
    FVector Origin, FVector Direction)
{
    // Ateş efektleri buraya — partikül, ses
}
