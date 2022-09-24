// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ThirdPerson_CPP.h"
#include "AttributeComponent.h"
#include "Containers/Array.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    //SetReplicates(true);//别处调用这个
    bReplicates = true;//构造函数中直接赋值
    
    BaseDamage = 10.f;
    LifeSpan = 5.f;
    LiveTime = 0.f;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    }

    if (!CollisionComponent)
    {
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
        CollisionComponent->InitSphereRadius(10.0f);
        CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
        CollisionComponent->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGrenade::OnOverlap);
        RootComponent = CollisionComponent;
    }

    if (!MovementComponent)
    {
        MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
        MovementComponent->SetUpdatedComponent(CollisionComponent);
        MovementComponent->InitialSpeed = 1000.f;
        MovementComponent->MaxSpeed = 0.0f;
        MovementComponent->bRotationFollowsVelocity = true;
        MovementComponent->bShouldBounce = true;
        MovementComponent->Bounciness = 0.3f;
        MovementComponent->ProjectileGravityScale = 1.0f;
    }

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RaicalForceComponent"));
    RadialForceComponent->SetupAttachment(RootComponent);
    //产生径向力的范围
    RadialForceComponent->Radius = 500.f;
    //力的大小
    RadialForceComponent->ForceStrength = 1000.f;
    RadialForceComponent->ImpulseStrength = 100000.f;

}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
    //Cout(this, GetOwner()->GetName());
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    LiveTime += DeltaTime;
    if (LiveTime >= LifeSpan)
    {
        Explode(BaseDamage);
    }
}

void AGrenade::FireInDirection(const FVector& ShootDirection)
{
    MovementComponent->Velocity = ShootDirection * MovementComponent->InitialSpeed;
}

void AGrenade::OnHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    FVector Velocity = MovementComponent->Velocity;
    //要想返回物理材质，要在物体碰撞选项里勾选Return Material On Move
    //或者使用射线追踪时设置LineTraceSingleByChannel的一个参数FCollisionQueryParams TraceParams(TEXT("TraceActor"), true, this);TraceParams.bReturnPhysicalMaterial = true;
    if (Hit.PhysMaterial.IsValid())
    {
        UPhysicalMaterial* HitPhysicalMaterial = Hit.PhysMaterial.Get();
        MovementComponent->Velocity = Velocity * HitPhysicalMaterial->Restitution * -1.f;
    }
}

void AGrenade::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor != GetOwner())
    {
        Explode(BaseDamage);
    }
}

void AGrenade::Explode(float Damage)
{
    UWorld* World = GetWorld();
    if (World)
    {
        UGameplayStatics::SpawnEmitterAtLocation(World, ExplodeParticle, this->GetTransform());
        RadialForceComponent->FireImpulse();//对周围物体产生冲量,与MovementComponent的Mass大小有关
        UGameplayStatics::ApplyRadialDamage(World, Damage, GetActorLocation(), RadialForceComponent->Radius, nullptr, TArray<AActor*>(), GetOwner(), nullptr, false, ECC_MAX);
        UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
    }
    Destroy();
}
