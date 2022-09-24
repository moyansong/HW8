// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ThirdPerson_CPP.h"
#include "AIController.h"

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					float Distance = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					//AIPawn离人多近时会停止
					bool bWithinRange = Distance < Range;
					bool bCanSee = false;//是否能看见TargetActor
					
					if (bWithinRange)
					{
						bCanSee = MyController->LineOfSightTo(TargetActor);//如果胶囊体碰撞类型是Pawn，则不会被追踪到
					}
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bCanSee);
				}
			}
		}
	}
}