#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    SetRootComponent(CollisionSphere);
    CollisionSphere->SetSphereRadius(10.f);
    CollisionSphere->SetCollisionProfileName(TEXT("Projectile"));

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
        TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed  = InitialSpeed;
    ProjectileMovement->MaxSpeed      = MaxSpeed;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale   = 0.1f;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectile::OnImpact);
}

void AProjectile::OnImpact(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HasAuthority()) return;

    if (OtherActor && OtherActor != GetOwner())
    {
        // IDamageable interface üzerinden hasar ver — LSP
        if (OtherActor->Implements<UDamageable>())
        {
            IDamageable::Execute_TakeDamage(OtherActor, Damage, GetOwner());
        }
    }

    Destroy();
}
