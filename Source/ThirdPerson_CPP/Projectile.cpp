// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "ThirdPerson_CPP.h"
#include "Components/SphereComponent.h"
#include "TwinblastCharacter.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AttributeComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SetReplicates(true);

    FirstHit = true;
    Damage = 40.f;

    if (!RootComponent)
    {
        //TEXT��������ĺ������������ʾ
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }

    if (!CollisionComponent)
    {
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        CollisionComponent->InitSphereRadius(3.0f);
        CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
        CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
        RootComponent = CollisionComponent;
    }

    if (!ProjectileMovementComponent)
    {
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        ProjectileMovementComponent->InitialSpeed = 100000.f;
        ProjectileMovementComponent->MaxSpeed = 0.0f;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = true;
        ProjectileMovementComponent->Bounciness = 0.3f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    }

    //���÷���������
    ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMeshComponent->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Game/ParagonTwinblast/FX/Meshes/Heroes/Twinblast/SM_MuzzleCore_02.SM_MuzzleCore_02'"));
    if (Mesh.Succeeded())
    {
        ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
    }

    //�����嵱��ײ��ı��빴ѡ��������µ�Simulate Physics
    //������Ҳ���Ե���ײ��в��ù�ѡ����ѡ��
    //�������������������Ҫ����������ײѡ����ΪOverlapAll���������ӵ�����������������ײ

    //���ò���,������������Mash��ģ�ֻ������ʵ�����ģ���������Material����Ҫ������ʵ����
    //�Ѿ�����ͼ�������Ĳ��������ˣ�����Ͳ�������
    /*static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/ParagonTwinblast/FX/Materials/Shells/M_Plasma_Shot_Shell.M_Plasma_Shot_Shell'"));
    if (Material.Succeeded())
    {
        ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
    }
    else GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Change the path of UMaterial")));
    ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
    ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
    ProjectileMeshComponent->SetupAttachment(RootComponent);*/

    ProjectileSpark = CreateDefaultSubobject<UParticleSystem>(TEXT("Emitter"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem>Spark(TEXT("ParticleSystem'/Game/ParagonTwinblast/FX/Particles/Abilities/Nitro/FX/P_TwinBlast_Nitro_HitWorld.P_TwinBlast_Nitro_HitWorld'"));
    if (Mesh.Succeeded())
    {
        ProjectileSpark = Spark.Object;
    }

    InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//����
void AProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    //��Ѫ
    if (OtherActor != this)
    {
        /*UAttributeComponent* AttributeComponent = Cast<UAttributeComponent>(OtherActor->GetComponentByClass(UAttributeComponent::StaticClass()));
        if (AttributeComponent)
        {
            AttributeComponent->ApplyHealthChange(GetOwner(), Damage);
        }*/

        ATwinblastCharacter* MyPawn = nullptr;
        float DamageAddition = 0.f;
        if (GetOwner())
        {
            MyPawn = Cast<ATwinblastCharacter>(GetOwner());
            if (MyPawn)
            {
                DamageAddition = MyPawn->DamageAddition;
            }
        }
        UGameplayStatics::ApplyPointDamage(OtherActor, Damage + DamageAddition, FVector::ZeroVector, FHitResult(), GetInstigatorController(), GetOwner(), nullptr);
    }

    //ʩ�ӳ���
    if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
    {
        //OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity / 10000.0f, Hit.ImpactPoint);  
    }
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSpark, this->GetTransform());
    Destroy();
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //��Ѫ
    if (OtherActor != this)
    {
        /*UAttributeComponent* AttributeComponent = Cast<UAttributeComponent>(OtherActor->GetComponentByClass(UAttributeComponent::StaticClass()));
        if (AttributeComponent)
        {
            AttributeComponent->ApplyHealthChange(Damage);
        }*/

        ATwinblastCharacter* MyPawn = nullptr;
        float DamageAddition = 0.f;
        if (GetOwner())
        {
            MyPawn = Cast<ATwinblastCharacter>(GetOwner());
            if (MyPawn)
            {
                DamageAddition = MyPawn->DamageAddition;
            }
        }
        UGameplayStatics::ApplyPointDamage(OtherActor, Damage + DamageAddition, FVector::ZeroVector, FHitResult(), GetInstigatorController(), GetOwner(), nullptr);
    }
    Destroy();
}