// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class THIRDPERSON_CPP_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//----------------------------------------------Component----------------------------------------------------------------------------------------------
	// ������ײ���
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		USphereComponent* CollisionComponent;

	// �������ƶ������
	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	//�������
	UPROPERTY(VisibleAnywhere, Category = Material)
		UMaterialInstanceDynamic* ProjectileMaterialInstance;

	//�������
	UPROPERTY(VisibleAnywhere, Category = Mesh)
		UStaticMeshComponent* ProjectileMeshComponent;

//----------------------------------------ParticleSystem----------------------------------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, Category = "Emitter")
		UParticleSystem* ProjectileSpark;

//-------------------------------------------Callback----------------------------------------------------------------------------------------------------
	//�������ϵĻص��������ͱ�����ﶼ�����������ƻص�
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

//-------------------------------------------Function----------------------------------------------------------------------------------------------------
	//����
	void FireInDirection(const FVector& ShootDirection);

//-------------------------------------------Parameter---------------------------------------------------------------------------------------------------
	//���ڵ�һ�δ�����ʱ��ʾ������Ч
	bool FirstHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		float Damage;
};
