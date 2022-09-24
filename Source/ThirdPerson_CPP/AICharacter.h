// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Containers/Array.h"
#include "AICharacter.generated.h"

class UPawnSensingComponent;
class UAttributeComponent;
class AProjectile;

UCLASS()
class THIRDPERSON_CPP_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

//protected:
	virtual void PostInitializeComponents() override;

//-------------------------------------------Component----------------------------------------------------
	//提供视觉,人物进入视觉范围才开始追击，视觉角度通过Peripheral Vision Angle设置
	UPROPERTY(VisibleAnywhere, Category = "Component")
		UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UAttributeComponent* AttributeComponent;

//-------------------------------------------Emitter----------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Emitter")
		UParticleSystem* GunSpark;

//-------------------------------------------Parameter----------------------------------------------------
	//攻击看得见的人？
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		bool bShouldAttackSeenActor = false;

	//下p放技能
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		float NextPartHealthProportion = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		TArray<TSubclassOf<class AAICharacter>> IgnoreActors;

	//击杀掉落奖励
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Drop")
		TArray<TSubclassOf<class AActor>> KillRewards;

//------------------------------------------Attack----------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
		bool bCanCloseAttack = false;

	//近战攻击
	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* CloseAttackMontage;

	//远程攻击
	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* RemoteAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Attack")
		float CloseDamage = 50.f;

	UPROPERTY(EditAnywhere, Category = "Attack")
		float CloseAttackRange = 200.f;

	UFUNCTION(BlueprintCallable)
		void CloseAttack(FVector HitLocation);

	UFUNCTION(BlueprintCallable)
		bool CanCloseAttack(float Distance);

	UFUNCTION(BlueprintCallable)
		void RemoteAttack(FVector HitLocation);

//-------------------------------------------Function--------------------------------------------------------------------
	UFUNCTION()
		void OnPawnSeen(APawn* Pawn);

	UFUNCTION(BlueprintCallable)
		void OnHealthChange(AActor* InstigatocActor, UAttributeComponent* OwningCopmonent, float NewHealth, float Damage);

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void Die();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void IncrementKillerScore(AActor* InstigatocActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
