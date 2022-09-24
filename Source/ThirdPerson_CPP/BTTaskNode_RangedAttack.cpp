// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_RangedAttack.h"
#include "AIController.h"
#include "ThirdPerson_CPP.h"
#include "AICharacter.h"
#include "Containers/Array.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"



EBTNodeResult::Type UBTTaskNode_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		AAICharacter* MyPawn = Cast<AAICharacter>(MyController->GetPawn());
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (MyPawn == nullptr || TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		float Distance = FVector::Distance(TargetActor->GetActorLocation(), MyPawn->GetActorLocation());
		if (MyPawn->CanCloseAttack(Distance))
		{
			MyPawn->CloseAttack(TargetActor->GetActorLocation());
		}
		else
		{
			FVector HitLocation = TargetActor->GetActorLocation();
			HitLocation.Z += 40.f;
			MyPawn->RemoteAttack(HitLocation);
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}