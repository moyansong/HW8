// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "Containers/Array.h"
#include "ThirdPerson_CPP.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);//指定行为树

	SetDefaultTargetActor();
}

void AMyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrTargetActor == nullptr)
	{
		SetDefaultTargetActor();
	}
}

void AMyAIController::SetDefaultTargetActor()
{
	//攻击玩家
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (PlayerPawn)
	//{
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	//}

	//攻击NPC
	TArray<AActor*> TargetActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetActorClass, TargetActors);
	if (TargetActors.Num() > 0 && TargetActors[0])
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", TargetActors[0]->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetActors[0]);
		CurrTargetActor = TargetActors[0]; 
	}
}