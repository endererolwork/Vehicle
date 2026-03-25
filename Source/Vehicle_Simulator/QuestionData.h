#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IQuestion.h"
#include "QuestionData.generated.h"

UCLASS(BlueprintType)
class VEHICLE_SIMULATOR_API UQuestionData : public UPrimaryDataAsset,
										   public IQuestion
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
	FText QuestionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
	TArray<FText> Answers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz",
			  meta = (ClampMin = 0))
	int32 CorrectAnswerIndex = 0;

	// IQuestion implementation
	virtual bool Evaluate_Implementation(int32 AnswerIndex) const override
	{
		return AnswerIndex == CorrectAnswerIndex;
	}

	virtual FText GetQuestionText_Implementation() const override
	{
		return QuestionText;
	}

	virtual TArray<FText> GetAnswers_Implementation() const override
	{
		return Answers;
	}
};