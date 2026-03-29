#include "ArmoredVehicle.h"

#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

#include "VehiclePhysicsComponent.h"
#include "TurretComponent.h"
#include "../Weapon/WeaponComponent.h"
#include "RoleComponent.h"

AArmoredVehicle::AArmoredVehicle()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetReplicatingMovement(true);

    // Collision root
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    SetRootComponent(CollisionBox);
    CollisionBox->SetBoxExtent(FVector(200.f, 100.f, 60.f));
    CollisionBox->SetSimulatePhysics(true);
    CollisionBox->SetCollisionProfileName(TEXT("Vehicle"));

    // Mesh
    VehicleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleMesh"));
    VehicleMesh->SetupAttachment(CollisionBox);

    // Third person camera
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CollisionBox);
    SpringArm->TargetArmLength = 600.f;
    SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
    SpringArm->bUsePawnControlRotation = true;

    ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
    ThirdPersonCamera->SetupAttachment(SpringArm);
    ThirdPersonCamera->SetActive(true);

    // Sight camera
    SightCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SightCamera"));
    SightCamera->SetupAttachment(VehicleMesh, TEXT("SightSocket"));
    SightCamera->SetActive(false);

    // Logic components — SRP
    PhysicsComponent  = CreateDefaultSubobject<UVehiclePhysicsComponent>(TEXT("PhysicsComponent"));
    TurretComponent   = CreateDefaultSubobject<UTurretComponent>(TEXT("TurretComponent"));
    WeaponComponent   = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
    RoleComponent     = CreateDefaultSubobject<URoleComponent>(TEXT("RoleComponent"));
}

void AArmoredVehicle::BeginPlay()
{
    Super::BeginPlay();
}

void AArmoredVehicle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Tick minimal — fizik PhysicsComponent'te
}

void AArmoredVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Enhanced Input kurulumu PlayerController'da yapılacak
}

void AArmoredVehicle::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AArmoredVehicle, Health);
    DOREPLIFETIME(AArmoredVehicle, CurrentRole);
}

// IDamageable
void AArmoredVehicle::TakeDamage_Implementation(float Amount, AActor* DamageCauser)
{
    if (!HasAuthority()) return;
    Health = FMath::Clamp(Health - Amount, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}

bool AArmoredVehicle::IsAlive_Implementation() const
{
    return Health > 0.f;
}

float AArmoredVehicle::GetHealth_Implementation() const
{
    return Health;
}

// IControllable
void AArmoredVehicle::SetInput_Implementation(
    float Throttle, float Steering, bool bBrake)
{
    if (IsLocallyControlled())
    {
        Server_SetInput(Throttle, Steering, bBrake);
    }
}

// IRoleSwappable
void AArmoredVehicle::OnRoleChanged_Implementation(EVehicleRole NewRole)
{
    if (HasAuthority())
    {
        CurrentRole = NewRole;
        OnVehicleRoleChanged.Broadcast(NewRole);
    }
    else
    {
        Server_SwitchRole(NewRole);
    }
}

EVehicleRole AArmoredVehicle::GetCurrentRole_Implementation() const
{
    return CurrentRole;
}

// Server RPCs
void AArmoredVehicle::Server_SetInput_Implementation(
    float Throttle, float Steering, bool bBrake)
{
    CurrentInput.Throttle = Throttle;
    CurrentInput.Steering = Steering;
    CurrentInput.bBrake   = bBrake;

    if (PhysicsComponent)
    {
        PhysicsComponent->SetInput_Implementation(Throttle, Steering, bBrake);
    }
}

bool AArmoredVehicle::Server_SetInput_Validate(
    float Throttle, float Steering, bool bBrake)
{
    // Anti-cheat: değerler makul aralıkta mı?
    return FMath::Abs(Throttle) <= 1.f && FMath::Abs(Steering) <= 1.f;
}

void AArmoredVehicle::Server_SwitchRole_Implementation(EVehicleRole NewRole)
{
    CurrentRole = NewRole;
    OnVehicleRoleChanged.Broadcast(NewRole);
}

bool AArmoredVehicle::Server_SwitchRole_Validate(EVehicleRole NewRole)
{
    return true;
}

// OnRep callbacks
void AArmoredVehicle::OnRep_Health()
{
    OnHealthChanged.Broadcast(Health);
}

void AArmoredVehicle::OnRep_CurrentRole()
{
    OnVehicleRoleChanged.Broadcast(CurrentRole);
}
