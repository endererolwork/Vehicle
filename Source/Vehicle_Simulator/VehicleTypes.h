#pragma once

#include "CoreMinimal.h"
#include "VehicleTypes.generated.h"

USTRUCT(BlueprintType)
struct FVehicleInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	float Throttle = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	float Steering = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	bool bBrake = false;
};

USTRUCT(BlueprintType)
struct FFireParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	FVector Origin = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	FVector Direction = FVector::ForwardVector;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	float MuzzleSpeed = 2000.f;
};

USTRUCT(BlueprintType)
struct FVehiclePhysicsConfig
{
	GENERATED_BODY()

	// Maksimum ileri hız (cm/s) — ~1500 ≈ 54 km/h
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float MaxForwardSpeed = 1500.f;

	// Maksimum geri hız (cm/s) — tanklar yavaş geri gider
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float MaxReverseSpeed = 500.f;

	// Hızlanma yumuşatma hızı (FInterpTo speed) — küçük = daha yavaş hızlanır
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float AccelerationRate = 1.5f;

	// Yavaşlama yumuşatma hızı (gaz bırakıldığında)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float DecelerationRate = 2.5f;

	// Maksimum yaw hızı (deg/s) — A/D ile ulaşılan terminal dönüş hızı
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float MaxYawRate = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float MaxBrakingForce = 500000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float SuspensionStiffness = 250000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float SuspensionDamping = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float SuspensionRestLength = 50.f;
};
