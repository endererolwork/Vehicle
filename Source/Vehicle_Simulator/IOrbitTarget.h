#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IOrbitTarget.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UOrbitTarget : public UInterface
{
	GENERATED_BODY()
};

class VEHICLE_SIMULATOR_API IOrbitTarget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void OnOrbitStart();

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void OnOrbitEnd();
};