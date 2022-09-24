// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

//������ʱ����ǰ���classҲ����
class USphereComponent;
class URadialForceComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class UPhysicalMaterial;
class USoundCue;
class UAnimSequence;
class UBlendSpace;
class UBlendSpace1D;
class AProjectile;
class UArrowComponent;
class UUserWidget;
class UTexture2D;
class UCameraShakeBase;

UCLASS()
class THIRDPERSON_CPP_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const override;

//------------------------------------Component----------------------------------------------------------
	// ������ײ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UProjectileMovementComponent* MovementComponent;

	//�궨�ӵ����ɵ�λ�úͷ���,����ƫ��ȥ��ͼ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* MuzzleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* ShellCaseComponent;

	//�������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UMaterialInstanceDynamic* MaterialInstance;

	//�������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		USkeletalMeshComponent* SKMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		TSubclassOf<UCameraShakeBase> FireCameraShake;

//--------------------------------------Animation--------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationSequence")
		UAnimSequence* AS_Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationSequence")
		UAnimSequence* AS_CrouchIdle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationSequence")
		UAnimSequence* AS_JumpStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationSequence")
		UAnimSequence* AS_JumpFall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationSequence")
		UAnimSequence* AS_JumpLand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationSequence")
		UAnimSequence* AS_JumpRecovery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendSpace")
		UBlendSpace* BS_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendSpace")
		UBlendSpace1D* BS_Stop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendSpace")
		UBlendSpace1D* BS_CrouchWalk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* FireMontage;

//--------------------------------------Sound--------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		USoundCue* ReloadSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		USoundCue* ProjectileDrySound;

//--------------------------------------Projectile--------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
		float CurrProjectileNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
		float ProjectileCostPerFire = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
		float MaxProjectileNum = 35.f;

//-----------------------------------------Emitter-----------------------------------------------------
	//��ǹ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Emitter")
		UParticleSystem* GunSpark;
	//����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Emitter")
		UParticleSystem* ShellCase;

//--------------------------------------Function--------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Fire(FVector HitLocation);//HitLoction��ʾ�ӵ�Ҫ������

	UFUNCTION(BlueprintCallable)
		bool CheckCanFire();

	UFUNCTION(BlueprintCallable)
		void Reload();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SimulateFire();//��������Ķ�����������Ч��

	//��׼
	UFUNCTION(BlueprintCallable)
		void TargetStart();
	UFUNCTION(BlueprintCallable)
		void TargetStop();

	UFUNCTION(BlueprintCallable)
		void CloseHit();
//--------------------------------------CallBack--------------------------------------------------------------------
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//��ս������
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
//--------------------------------------Parameter--------------------------------------------------------------------
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		APawn* MyPawn;*/
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		float Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		float RecoilForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		float Damage;//��ս�����˺�

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		float FireMontagePlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		UTexture2D* WeaponIcon;//����ͼ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		bool bCanContinuousFire = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		bool bHasSight = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		bool bIstargeting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		float TargetFOV = 20.f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		//bool bCanPickup = false;

	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_IsSimulating", BlueprintReadWrite)
		int64 IsSimulating = 1;
	UFUNCTION()
		void OnRep_IsSimulating();

//--------------------------------------Widget--------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> TargetWidgetClass;
	UPROPERTY()
		UUserWidget* TargetWidget;

};
