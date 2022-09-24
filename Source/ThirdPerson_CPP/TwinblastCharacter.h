// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "GameFramework/Character.h"
#include "TwinblastCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChangeSignature, AActor*, InstigatocActor);

class USpringArmComponent;
class UCameraComponent;
class UAttributeComponent;
class UWeaponComponent;
class AProjectile;
class AGrenade;
class AWeapon;
class UUserWidget;
class UWorldUserWidget;
class UAudioComponent;
class USoundCue;

UCLASS()
class THIRDPERSON_CPP_API ATwinblastCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATwinblastCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const override;

//-------------------------------------------Weapon--------------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AGrenade> GrenadeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TArray<TSubclassOf<class AWeapon>> WeaponClassArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		int32 CurrWeaponIndex = 0;

	UPROPERTY(BlueprintReadWrite)
		AWeapon* Weapon;
	UFUNCTION()
		void OnRep_Weapon();

protected:
//-------------------------------------------Input--------------------------------------------------------------
	UFUNCTION()
		void OnResetVR();

	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
		void ChangeWalkSpeed(float Speed);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION()
		void TurnAtRate(float Rate);

	UFUNCTION()
		void LookUpAtRate(float Rate);

	UFUNCTION()
		void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION()
		void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION(BlueprintCallable)
		void JumpStart();

	UFUNCTION()
		void JumpStop();

	UFUNCTION(BlueprintCallable)
		void CrouchStart();

	UFUNCTION(BlueprintCallable)
		void CrouchStop();

	//ͨ�������ֻ���Ļƽ����ת�ӽ�
	UFUNCTION()
		void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	TouchData	TouchItem;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

//----------------------------------------Prarmeter------------------------------------------------------------
	//����һ��ת���̧ͷ�ĽǶȴ�С
	UPROPERTY(EditAnywhere)
		float BaseTurnRate;
	UPROPERTY(EditAnywhere)
		float BaseLookUpRate;

	//����ٶ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameter")
		float MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameter")
		float MaxRunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameter")
		FTimerHandle FireTimer;

	//��̬ʮ��׼�ǵķ�ɢ�̶�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Parameter")
		float CrosshairSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		FVector DeathLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		bool bIsRagdolling = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameter")
		FVector PelvisOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Buff")
		float DamageAddition = 0.f;

	/*UPROPERTY(Replicated, ReplicatedUsing = "OnRep_bIsTimeToPlayMontage")
		bool bIsTimeToPlayMontage = false;
	UFUNCTION()
		void OnRep_bIsTimeToPlayMontage(UAnimMontage* Montage);*/
//-----------------------------------------ParticleSystem-----------------------------------------------------

//--------------------------------------------Sound-----------------------------------------------------

//-----------------------------------------Component-----------------------------------------------------
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		UAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		UWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UAudioComponent* AudioComponent;

//----------------------------------------Montage----------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* GetUpMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* ThrowMontage_FullBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* ThrowMontage_UpperBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* StartMontage;

//------------------------------------------Widget--------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> AttributeWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		UUserWidget* AttributeWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> MoblieControlWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		UUserWidget* MoblieControlWidget;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> HealthBarWidgetClass;
	UPROPERTY()
		UWorldUserWidget* HealthBarWidget;*/

//-----------------------------------------------����--------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
		void Fire();
	UFUNCTION(BlueprintCallable)
		void FireStart();
	UFUNCTION(BlueprintCallable)
		void FireStop();

	UFUNCTION(BlueprintCallable)
		void Reload();

	UFUNCTION(BlueprintCallable)
		void SpawnGrenade();

	UFUNCTION(BlueprintCallable)
		void TargetStart();
	UFUNCTION(BlueprintCallable)
		void TargetStop();

	UFUNCTION(BlueprintCallable)
		void ThrowStart();

	UFUNCTION(BlueprintCallable)
		void ThrowStop();

	UFUNCTION(BlueprintCallable)
		void Crouching();

	UFUNCTION(BlueprintCallable)
		bool AddWeapon(TSubclassOf<class AWeapon> NewWeaponClass);

	UFUNCTION(BlueprintCallable)
		void ApplyWeaponChange();

//-----------------------------------------------Function--------------------------------------------------------------------
	UPROPERTY(BlueprintAssignable)
		FOnWeaponChangeSignature OnWeaponChange;
	
	UFUNCTION(BlueprintCallable)
		void OnHealthChange(AActor* InstigatocActor, UAttributeComponent* OwningCopmonent, float NewHealth, float Damage);

	UFUNCTION(BlueprintCallable)
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable)
		void Die();

	UFUNCTION(BlueprintCallable)
		void Reborn();

	UFUNCTION(BlueprintCallable)
		void UpdateCapsuleComponentLocation();//��Mesh������Capsule��
//-----------------------------------------------Server--------------------------------------------------------------------
	//Reliable��֤���ݰ��ܵ����������ɿ���
	UFUNCTION(Server, Reliable)
		void ServerThrowStart();//ֻ�ܶ���XXX_Implementation
	UFUNCTION(Server, Reliable)
		void ServerFire();
	UFUNCTION(Server, Reliable)
		void ServerApplyWeaponChange();
	UFUNCTION(NetMulticast, Reliable)
		void MultcastRPCWeaponChange();
};
