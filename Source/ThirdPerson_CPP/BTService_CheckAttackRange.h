// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_CPP_API UBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
protected:

	//ѡ��Service���ұ�����ϻ���ʾ,Ȼ����BT���е����ServiceΪ�ñ���ѡ���Ӧ�ĺڰ��ֵ
	// �������ͼ�������ñ�����Ҫ��ѡInstanceEditable��ExposeOnSpawn��
	UPROPERTY(EditAnywhere, Category = "AI")
		FBlackboardKeySelector AttackRangeKey;

	UPROPERTY(EditAnywhere, Category = "AI")
		float Range = 1000.f;

	//ÿ֡��������
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
