#include "ArmoredVehicle.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
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

    // Root collision
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    SetRootComponent(CollisionBox);
    CollisionBox->SetBoxExtent(FVector(200.f, 100.f, 60.f));
    CollisionBox->SetSimulatePhysics(true);
    CollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
    CollisionBox->SetLinearDamping(0.5f);
    CollisionBox->SetAngularDamping(8.0f);

    // Gövde mesh
    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    BodyMesh->SetupAttachment(CollisionBox);
    // Mesh'in görsel ilerisi Y eksenindeyse -90° döndürerek actor X (forward) ile hizala
    BodyMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    // Kule — gövdeye bağlı
    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
    TurretMesh->SetupAttachment(BodyMesh);
    TurretMesh->SetRelativeLocation(FVector(0.f, 0.f, 60.f));

    // Namlu — kuleye bağlı
    BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
    BarrelMesh->SetupAttachment(TurretMesh);
    BarrelMesh->SetRelativeLocation(FVector(200.f, 0.f, 0.f));

    // Kamera
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CollisionBox);
    SpringArm->TargetArmLength = 600.f;
    SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
    SpringArm->bUsePawnControlRotation = true;

    ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
    ThirdPersonCamera->SetupAttachment(SpringArm);
    ThirdPersonCamera->SetActive(true);

    SightCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SightCamera"));
    SightCamera->SetupAttachment(BarrelMesh);
    SightCamera->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
    SightCamera->SetActive(false);

    // Logic components
    PhysicsComponent = CreateDefaultSubobject<UVehiclePhysicsComponent>(TEXT("PhysicsComponent"));
    TurretComponent  = CreateDefaultSubobject<UTurretComponent>(TEXT("TurretComponent"));
    WeaponComponent  = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
    RoleComponent    = CreateDefaultSubobject<URoleComponent>(TEXT("RoleComponent"));
}

void AArmoredVehicle::BeginPlay()
{
    Super::BeginPlay();

    // TurretComponent'a mesh referanslarını ilet
    if (TurretComponent)
    {
        TurretComponent->Initialize(TurretMesh, BarrelMesh);
    }
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

void AArmoredVehicle::ToggleCamera()
{
    if (!ThirdPersonCamera || !SightCamera) return;

    const bool bSightWasActive = SightCamera->IsActive();
    ThirdPersonCamera->SetActive(bSightWasActive);
    SightCamera->SetActive(!bSightWasActive);
}

FVector AArmoredVehicle::GetMuzzleLocation() const
{
    if (BarrelMesh)
    {
        return BarrelMesh->GetComponentLocation();
    }
    return GetActorLocation() + FVector(0.f, 0.f, 60.f);
}

FVector AArmoredVehicle::GetMuzzleForwardVector() const
{
    if (BarrelMesh)
    {
        return BarrelMesh->GetForwardVector();
    }
    return GetActorForwardVector();
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
