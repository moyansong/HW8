// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "Projectile.h"
#include "Grenade.h"
#include "Weapon.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	WeaponClassArray.Init(nullptr, 2);
	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UWeaponComponent::AddWeapon(TSubclassOf<class AWeapon> NewWeaponClass)
{
	bool bIsHaved = WeaponClassArray.Contains(NewWeaponClass);
	if (bIsHaved) return false;

	WeaponClassArray.Emplace(NewWeaponClass);
	return true;
}

void UWeaponComponent::ApplyWeaponChange()
{
	++CurrWeaponIndex;
	if (CurrWeaponIndex == WeaponClassArray.Num())
	{
		CurrWeaponIndex = 0;
	}

	if (Weapon)
	{
		Weapon->Destroy();
		Weapon = nullptr;
	}

	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (WeaponClassArray[CurrWeaponIndex] && OwnerMesh)
	{
		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClassArray[CurrWeaponIndex], Owner->GetActorTransform());
		//Weapon->SetOwner(OwnerPawn);
		//Mesh必须是根组件才能绑到Socket上
		Weapon->AttachToComponent(OwnerMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true), FName(TEXT("WeaponSocket")));
	}

	//OnWeaponChange.Broadcast(Owner);
}

