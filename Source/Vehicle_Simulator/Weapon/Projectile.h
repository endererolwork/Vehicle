#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/IDamageable.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class VEHICLE_SIMULATOR_API AProjectile : public AActor
{
    GENERATED_BODY()

public:
    AProjectile();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnImpact(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    UPROPERTY(VisibleAnywhere, Category = "Projectile")
    TObjectPtr<USphereComponent> CollisionSphere;

    UPROPERTY(VisibleAnywhere, Category = "Projectile")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    float Damage = 50.f;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    float InitialSpeed = 2000.f;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    float MaxSpeed = 2000.f;
};
