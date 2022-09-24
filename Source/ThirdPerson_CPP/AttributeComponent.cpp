// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "ThirdPerson_CPP.h"
#include "TwinblastCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	MaxHealth = 100.0f;
	Health = MaxHealth;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAttributeComponent::ApplyHealthChange(AActor* InstigationActor, float Damage)
{
	if (!GetOwner()->HasAuthority())
	{	

	}
	Health -= Damage;
	Health = UKismetMathLibrary::FMax(0.0, Health);
	Health = UKismetMathLibrary::FMin(Health, MaxHealth);
	MultcastRPCHealthChange(InstigationActor, Health, Damage);
	return true;
}

bool UAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

void UAttributeComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAttributeComponent, Health);
	DOREPLIFETIME(UAttributeComponent, MaxHealth);
	//DOREPLIFETIME_CONDITION(UAttributeComponent, MaxHealth, COND_InitialOnly);
}

void UAttributeComponent::MultcastRPCHealthChange_Implementation(AActor* InstigationActor, float NewHealth, float Damage)
{
	OnHealthChanged.Broadcast(InstigationActor, this, Health, Damage);
}

void UAttributeComponent::OnRep_Health()
{
	if (Health <= 0.f)
	{
		ATwinblastCharacter* MyCharacter = Cast<ATwinblastCharacter>(GetOwner());
		MyCharacter->Die();
	}
}