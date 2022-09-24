// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChangeSignature, AActor*, InstigatocActor);

class AGrenade;
class AWeapon;
class AChararcter;
class USkeletalMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSON_CPP_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "Owner")
		USkeletalMeshComponent* OwnerMesh;
//--------------------------------------------Weapon--------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<class AGrenade> GrenadeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TArray<TSubclassOf<class AWeapon>> WeaponClassArray;

	UPROPERTY(BlueprintReadWrite)
		AWeapon* Weapon;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		int32 CurrWeaponIndex = 0;
//--------------------------------------------Function------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
		bool AddWeapon(TSubclassOf<class AWeapon> NewWeaponClass);

	UFUNCTION(BlueprintCallable)
		void ApplyWeaponChange();

	/*UPROPERTY(BlueprintAssignable)
		FOnWeaponChangeSignature OnWeaponChange;*/
};
