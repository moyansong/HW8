// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedSignature, AActor*, InstigatocActor, UAttributeComponent*, OwningCopmonent, float, NewHealth, float, Damage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSON_CPP_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Arrtribute")
		bool ApplyHealthChange(AActor* InstigationActor, float Damage);//负责广播事件

	UFUNCTION(BlueprintCallable)
		bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
		float HealthProportion() const { return Health / MaxHealth * 100.f;  }

public://Attribute

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, ReplicatedUsing = "OnRep_Health", Category = "Attribute")
		float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
		float MaxHealth;

	UFUNCTION()
		void OnRep_Health();

	UPROPERTY(BlueprintAssignable)
		FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(NetMulticast, Reliable)
		void MultcastRPCHealthChange(AActor* InstigationActor, float NewHealth, float Damage);
};
