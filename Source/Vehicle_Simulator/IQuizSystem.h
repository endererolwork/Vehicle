#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IQuizSystem.generated.h"

class UQuestionData;

UINTERFACE(MinimalAPI, Blueprintable)
class UQuizSystem : public UInterface
{
	GENERATED_BODY()
};

class VEHICLE_SIMULATOR_API IQuizSystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Quiz")
	UQuestionData* GetNextQuestion();

	UFUNCTION(BlueprintNativeEvent, Category = "Quiz")
	bool HasMoreQuestions() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Quiz")
	int32 GetCorrectCount() const;
};