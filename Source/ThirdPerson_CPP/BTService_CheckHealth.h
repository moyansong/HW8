// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_CPP_API UBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
		FBlackboardKeySelector ShouldUseSkillKey;

	UPROPERTY(EditAnywhere, Category = "AI")
		float NextPartHealthProportion = 90.f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
