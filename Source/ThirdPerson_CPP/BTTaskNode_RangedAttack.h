// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
//#include "Windows/AllowWindowsPlatformTypes.h"
//#include "Windows/PreWindowsApi.h"
//#include "ApexDefs.h"
//#include "Windows/PostWindowsApi.h"
//#include "Windows/HideWindowsPlatformTypes.h"
#include "BTTaskNode_RangedAttack.generated.h"

//根据距离选择近战攻击或远程攻击
UCLASS()
class THIRDPERSON_CPP_API UBTTaskNode_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
