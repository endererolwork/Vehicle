#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interfaces/IWeapon.h"
#include "../VehicleTypes.h"
#include "WeaponComponent.generated.h"

class AProjectile;

UCLASS(ClassGroup=(Weapon), meta=(BlueprintSpawnableComponent))
class VEHICLE_SIMULATOR_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();

    // DIP: IWeapon interface üzerinden ateş eder, somut tip bilmez
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void RequestFire(const FFireParams& Params);

    // Set active weapon by class (creates UObject that implements IWeapon)
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetActiveWeaponByClass(TSubclassOf<UObject> WeaponClass);

    UFUNCTION(BlueprintPure, Category = "Weapon")
    UObject* GetActiveWeapon() const { return ActiveWeapon.GetObject(); }

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_Fire(const FFireParams& Params);

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_FireEffects(FVector Origin, FVector Direction);

    // DIP: aktif silah interface üzerinden - somut tip bilmez
    UPROPERTY()
    TScriptInterface<IWeapon> ActiveWeapon;
};
