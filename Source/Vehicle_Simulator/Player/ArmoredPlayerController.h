#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Interfaces/IRoleSwappable.h"
#include "ArmoredPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class AArmoredVehicle;

UCLASS()
class VEHICLE_SIMULATOR_API AArmoredPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AArmoredPlayerController();

	// Role switching
	UFUNCTION(BlueprintCallable, Category = "Role")
	void SwitchToDriverRole();

	UFUNCTION(BlueprintCallable, Category = "Role")
	void SwitchToGunnerRole();

	UFUNCTION(BlueprintPure, Category = "Role")
	EVehicleRole GetCurrentRole() const { return CurrentRole; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	// Input Mapping Contexts
	UPROPERTY(EditDefaultsOnly, Category = "Input|Contexts")
	TObjectPtr<UInputMappingContext> IMC_Driver;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Contexts")
	TObjectPtr<UInputMappingContext> IMC_Gunner;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Contexts")
	TObjectPtr<UInputMappingContext> IMC_Common;

	// Input Actions - Driver
	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions|Driver")
	TObjectPtr<UInputAction> IA_Throttle;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions|Driver")
	TObjectPtr<UInputAction> IA_Steering;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions|Driver")
	TObjectPtr<UInputAction> IA_Brake;

	// Input Actions - Gunner
	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions|Gunner")
	TObjectPtr<UInputAction> IA_RotateTurret;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions|Gunner")
	TObjectPtr<UInputAction> IA_Fire;

	// Input Actions - Common
	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions|Common")
	TObjectPtr<UInputAction> IA_SwitchRole;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions|Common")
	TObjectPtr<UInputAction> IA_SwitchCamera;

private:
	void ApplyInputMappingContext(EVehicleRole NewRole);

	// Input callbacks - Driver
	void OnThrottle(const struct FInputActionValue& Value);
	void OnSteering(const struct FInputActionValue& Value);
	void OnBrake(const struct FInputActionValue& Value);

	// Input callbacks - Gunner
	void OnRotateTurret(const struct FInputActionValue& Value);
	void OnFire(const struct FInputActionValue& Value);

	// Input callbacks - Common
	void OnSwitchRole(const struct FInputActionValue& Value);
	void OnSwitchCamera(const struct FInputActionValue& Value);

	UPROPERTY()
	TObjectPtr<AArmoredVehicle> ControlledVehicle;

	EVehicleRole CurrentRole = EVehicleRole::None;

	// Input state
	float CurrentThrottle = 0.0f;
	float CurrentSteering = 0.0f;
	bool bCurrentBrake = false;
};
