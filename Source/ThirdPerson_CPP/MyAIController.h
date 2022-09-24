// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class THIRDPERSON_CPP_API AMyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		TSubclassOf<class AActor> TargetActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		AActor* CurrTargetActor;

	UFUNCTION(BlueprintCallable)
		void SetDefaultTargetActor();

};
