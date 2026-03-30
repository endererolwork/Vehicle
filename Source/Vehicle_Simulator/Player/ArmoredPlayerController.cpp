#include "ArmoredPlayerController.h"
#include "../Vehicle/ArmoredVehicle.h"
#include "../Vehicle/TurretComponent.h"
#include "../Weapon/WeaponComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"

AArmoredPlayerController::AArmoredPlayerController()
{
	bReplicates = true;
}

void AArmoredPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Default role: Driver
	if (IsLocalController())
	{
		SwitchToDriverRole();
	}
}

void AArmoredPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledVehicle = Cast<AArmoredVehicle>(InPawn);
}

void AArmoredPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInput) return;

	// Driver inputs
	if (IA_Throttle)
		EnhancedInput->BindAction(IA_Throttle, ETriggerEvent::Triggered, this, &AArmoredPlayerController::OnThrottle);
	if (IA_Steering)
		EnhancedInput->BindAction(IA_Steering, ETriggerEvent::Triggered, this, &AArmoredPlayerController::OnSteering);
	if (IA_Brake)
		EnhancedInput->BindAction(IA_Brake, ETriggerEvent::Triggered, this, &AArmoredPlayerController::OnBrake);

	// Gunner inputs
	if (IA_RotateTurret)
		EnhancedInput->BindAction(IA_RotateTurret, ETriggerEvent::Triggered, this, &AArmoredPlayerController::OnRotateTurret);
	if (IA_Fire)
		EnhancedInput->BindAction(IA_Fire, ETriggerEvent::Started, this, &AArmoredPlayerController::OnFire);

	// Common inputs
	if (IA_SwitchRole)
		EnhancedInput->BindAction(IA_SwitchRole, ETriggerEvent::Started, this, &AArmoredPlayerController::OnSwitchRole);
	if (IA_SwitchCamera)
		EnhancedInput->BindAction(IA_SwitchCamera, ETriggerEvent::Started, this, &AArmoredPlayerController::OnSwitchCamera);
}

void AArmoredPlayerController::SwitchToDriverRole()
{
	CurrentRole = EVehicleRole::Driver;
	ApplyInputMappingContext(EVehicleRole::Driver);

	if (ControlledVehicle)
	{
		ControlledVehicle->OnRoleChanged_Implementation(EVehicleRole::Driver);
	}
}

void AArmoredPlayerController::SwitchToGunnerRole()
{
	CurrentRole = EVehicleRole::Gunner;
	ApplyInputMappingContext(EVehicleRole::Gunner);

	if (ControlledVehicle)
	{
		ControlledVehicle->OnRoleChanged_Implementation(EVehicleRole::Gunner);
	}
}

void AArmoredPlayerController::ApplyInputMappingContext(EVehicleRole NewRole)
{
	if (!IsLocalController()) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!Subsystem) return;

	// Clear all contexts
	Subsystem->ClearAllMappings();

	// Add common context (always active)
	if (IMC_Common)
	{
		Subsystem->AddMappingContext(IMC_Common, 0);
	}

	// Add role-specific context
	switch (NewRole)
	{
	case EVehicleRole::Driver:
		if (IMC_Driver)
		{
			Subsystem->AddMappingContext(IMC_Driver, 1);
		}
		break;

	case EVehicleRole::Gunner:
		if (IMC_Gunner)
		{
			Subsystem->AddMappingContext(IMC_Gunner, 1);
		}
		break;

	default:
		break;
	}
}

// Driver callbacks
void AArmoredPlayerController::OnThrottle(const FInputActionValue& Value)
{
	if (CurrentRole != EVehicleRole::Driver || !ControlledVehicle) return;

	CurrentThrottle = Value.Get<float>();
	ControlledVehicle->SetInput_Implementation(CurrentThrottle, CurrentSteering, bCurrentBrake);
}

void AArmoredPlayerController::OnSteering(const FInputActionValue& Value)
{
	if (CurrentRole != EVehicleRole::Driver || !ControlledVehicle) return;

	CurrentSteering = Value.Get<float>();
	ControlledVehicle->SetInput_Implementation(CurrentThrottle, CurrentSteering, bCurrentBrake);
}

void AArmoredPlayerController::OnBrake(const FInputActionValue& Value)
{
	if (CurrentRole != EVehicleRole::Driver || !ControlledVehicle) return;

	bCurrentBrake = Value.Get<bool>();
	ControlledVehicle->SetInput_Implementation(CurrentThrottle, CurrentSteering, bCurrentBrake);
}

// Gunner callbacks
void AArmoredPlayerController::OnRotateTurret(const FInputActionValue& Value)
{
	if (CurrentRole != EVehicleRole::Gunner || !ControlledVehicle) return;

	const FVector2D TurretInput = Value.Get<FVector2D>();
	
	UTurretComponent* Turret = ControlledVehicle->GetTurretComponent();
	if (Turret)
	{
		const float DeltaTime = GetWorld()->GetDeltaSeconds();
		Turret->RotateTurret(TurretInput.X, TurretInput.Y, DeltaTime);
	}
}

void AArmoredPlayerController::OnFire(const FInputActionValue& Value)
{
	if (CurrentRole != EVehicleRole::Gunner || !ControlledVehicle) return;

	UWeaponComponent* Weapon = ControlledVehicle->GetWeaponComponent();
	if (Weapon)
	{
		FFireParams Params;
		Params.Origin    = ControlledVehicle->GetMuzzleLocation();
		Params.Direction = ControlledVehicle->GetMuzzleForwardVector();
		Params.MuzzleSpeed = 2000.0f;

		Weapon->RequestFire(Params);
	}
}

// Common callbacks
void AArmoredPlayerController::OnSwitchRole(const FInputActionValue& Value)
{
	if (CurrentRole == EVehicleRole::Driver)
	{
		SwitchToGunnerRole();
	}
	else
	{
		SwitchToDriverRole();
	}
}

void AArmoredPlayerController::OnSwitchCamera(const FInputActionValue& Value)
{
	if (ControlledVehicle)
	{
		ControlledVehicle->ToggleCamera();
	}
}
