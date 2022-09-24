// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "Projectile.h"
#include "ThirdPerson_CPP.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttributeComponent.h"
#include "BrainComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (MovementComponent)
	{
		MovementComponent->bUseRVOAvoidance = true;//防止同一路径上的AI互相阻挡
	}

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	/*AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC && TargetActor)
	{
		UBlackboardComponent* BBComponent = AIC->GetBlackboardComponent();
		BBComponent->SetValueAsVector("MoveToLocation", TargetActor->GetActorLocation());
		BBComponent->SetValueAsObject("TargetActor", TargetActor);
	}*/
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (bShouldAttackSeenActor)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAICharacter::OnPawnSeen);
	}

	AttributeComponent->OnHealthChanged.AddDynamic(this, &AAICharacter::OnHealthChange);
}

void AAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		UBlackboardComponent* BBComponent = AIC->GetBlackboardComponent();
		BBComponent->SetValueAsObject("TargetActor", Pawn);
	}
}

bool AAICharacter::CanCloseAttack(float Distance)
{
	return bCanCloseAttack && Distance <= CloseAttackRange && CloseAttackMontage;
}

void AAICharacter::CloseAttack(FVector HitLocation)
{
	if (!bCanCloseAttack)
	{
		return;
	}
	UWorld* World = GetWorld();
	TArray<AActor*> Ignores;
	for (int i = 0; i < IgnoreActors.Num(); ++i)
	{
		TArray<AActor*> TempIgnores;
		UGameplayStatics::GetAllActorsOfClass(World, IgnoreActors[i], TempIgnores);
		for (int j = 0; j < TempIgnores.Num(); ++j)
		{
			Ignores.Add(TempIgnores[j]);
		}
	}
	UGameplayStatics::ApplyRadialDamage(World, CloseDamage, HitLocation, 10.f, nullptr, Ignores, this);
	PlayAnimMontage(CloseAttackMontage, 1.f);
}

void AAICharacter::RemoteAttack(FVector HitLocation)
{
	FVector MuzzleLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector Direction = HitLocation - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();

	if (ProjectileClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GunSpark, MuzzleLocation, MuzzleRotation);
		if (Projectile)
		{
			Projectile->SetOwner(this);
			PlayAnimMontage(RemoteAttackMontage, 1.f);
		}
	}
}

void AAICharacter::OnHealthChange(AActor* InstigatocActor, UAttributeComponent* OwningCopmonent, float NewHealth, float Damage)
{
	if (NewHealth <= 0.f)
	{
		Die();
		IncrementKillerScore(InstigatocActor);
	}
}

void AAICharacter::Die()
{
	AAIController* MyController = Cast<AAIController>(GetController());
	if (MyController)
	{
		UBrainComponent* BrainComponent = MyController->GetBrainComponent();
		if (BrainComponent)
		{
			BrainComponent->StopLogic("Killed");
		}
	}

	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(4.f);

	int32 Index = FMath::RandRange(0.f, (float)KillRewards.Num());
	if (Index < KillRewards.Num() && KillRewards[Index])
	{
		AActor* KillReward = GetWorld()->SpawnActor<AActor>(KillRewards[Index], GetActorLocation(), FRotator::ZeroRotator, FActorSpawnParameters());
		KillReward->SetLifeSpan(5.f);
	}
}

void AAICharacter::IncrementKillerScore_Implementation(AActor* InstigatocActor)
{

}

float AAICharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if ((IgnoreActors.Num() && DamageCauser && IgnoreActors.Find(DamageCauser->GetClass()) >= 0) || AttributeComponent == nullptr || !AttributeComponent->IsAlive())
	{
		return 0.f;
	}

	//受到伤害转火攻击的人
	AttributeComponent->ApplyHealthChange(DamageCauser, Damage);
	AAIController* AIC = Cast<AAIController>(GetController());
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (AIC && DamageCauser && !Cast<AAICharacter>(DamageCauser))
	{
		UBlackboardComponent* BBComponent = AIC->GetBlackboardComponent();
		if (BBComponent)
		{
			BBComponent->SetValueAsVector("MoveToLocation", DamageCauser->GetActorLocation());
			BBComponent->SetValueAsObject("TargetActor", DamageCauser);
		}
	}
	return Damage;
}
