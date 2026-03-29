#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnginePartData.generated.h"

UCLASS(BlueprintType)
class ENGINEEDUCATION_API UEnginePartData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part")
	FName PartID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part")
	FText PartName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part",
			  meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part")
	TSoftObjectPtr<UStaticMesh> PartMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part")
	FLinearColor HighlightColor = FLinearColor(1.f, 0.6f, 0.f, 1.f);

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("EnginePart", PartID);
	}
};