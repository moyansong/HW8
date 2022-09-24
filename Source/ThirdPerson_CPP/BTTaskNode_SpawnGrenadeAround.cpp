// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SpawnGrenadeAround.h"
#include "AIController.h"
#include "ThirdPerson_CPP.h"
#include "AttributeComponent.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_SpawnGrenadeAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		AAICharacter* MyPawn = Cast<AAICharacter>(MyController->GetPawn());
		UWorld* World = GetWorld();
		if (MyPawn == nullptr || World == nullptr || GrenadeClass == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector PawnLocation = MyPawn->GetActorLocation();
		FVector Radius(500.f, 0.f, 0.f);
		FVector RotateAxis(0.f, 0.f, 1.f);
		FRotator SpawnRoation(0.f, 0.f, MyPawn->GetActorRotation().Yaw);
		float SpawnAngleOffset = 45.f;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = MyPawn;
		SpawnParams.Instigator = MyPawn;
		
		for (int i = 0; i < 360.f / SpawnAngleOffset; ++i)
		{
			Radius = Radius.RotateAngleAxis(SpawnAngleOffset, RotateAxis);
			SpawnRoation.Yaw += SpawnAngleOffset;
			World->SpawnActor<AActor>(GrenadeClass, PawnLocation + Radius, SpawnRoation, SpawnParams);
		}
		
		MyPawn->NextPartHealthProportion = (int)MyPawn->AttributeComponent->HealthProportion() / 10 * 10.f;
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}