#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IQuestion.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UQuestion : public UInterface
{
	GENERATED_BODY()
};

class VEHICLE_SIMULATOR_API IQuestion
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Quiz")
	bool Evaluate(int32 AnswerIndex) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Quiz")
	FText GetQuestionText() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Quiz")
	TArray<FText> GetAnswers() const;
};