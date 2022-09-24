// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_SpawnGrenadeAround.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_CPP_API UBTTaskNode_SpawnGrenadeAround : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<class AActor> GrenadeClass;
};
