// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ThirdPerson_CPP.h"
#include "AICharacter.h"
#include "AttributeComponent.h"
#include "AIController.h"

void UBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController))
		{
			AAICharacter* MyPawn = Cast<AAICharacter>(MyController->GetPawn());
			if (ensure(MyPawn))
			{
				bool bShouldUseSkill = MyPawn->AttributeComponent->HealthProportion() < MyPawn->NextPartHealthProportion;
				BlackboardComp->SetValueAsBool(ShouldUseSkillKey.SelectedKeyName, bShouldUseSkill);
			}
		}
	}
}