// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "ThirdPerson_CPP.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AttributeComponent.h"
#include "Containers/Array.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimSequence.h"
#include "Animation/BlendSpace.h"
#include "Animation/BlendSpace1D.h"
#include "Projectile.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "TwinblastCharacter.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    Range = 10000000.f;
    RecoilForce = 1.f;
    Damage = 50.f;
    FireMontagePlayRate = 1.f;
    CurrProjectileNum = MaxProjectileNum;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    }

    //MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    //RootComponent = MeshComponent;

    SKMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SKMeshComponent->SetupAttachment(RootComponent);
    //RootComponent = SKMeshComponent;
  

    if (!CollisionComponent)
    {
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
        CollisionComponent->InitSphereRadius(30.0f);
        CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
        CollisionComponent->SetupAttachment(RootComponent);
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlap);
        CollisionComponent->OnComponentHit.AddDynamic(this, &AWeapon::OnHit);
        //RootComponent = CollisionComponent;
    }

    if (!MovementComponent)
    {
        MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
        MovementComponent->SetUpdatedComponent(RootComponent);
        MovementComponent->InitialSpeed = 0.f;
        MovementComponent->MaxSpeed = 0.0f;
        MovementComponent->bRotationFollowsVelocity = false;
        MovementComponent->bShouldBounce = false;
        MovementComponent->Velocity = FVector(0.f, 0.f, 0.f);
        //MovementComponent->Bounciness = 0.f;
        MovementComponent->ProjectileGravityScale = 0.f;
    }

    MuzzleComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleComponent"));
    MuzzleComponent->SetupAttachment(RootComponent);
    ShellCaseComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ShellCaseComponent"));
    ShellCaseComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::TargetStart()
{
    if (bHasSight)
    {
        if (TargetWidgetClass != nullptr)
        {
            TargetWidget = CreateWidget<UUserWidget>(GetWorld(), TargetWidgetClass);
            if (TargetWidget != nullptr)
            {
                TargetWidget->AddToViewport(-1);
                bIstargeting = true;
            }
        }
    }
}

void AWeapon::TargetStop()
{
    if (bHasSight)
    {
        if (TargetWidget != nullptr)
        {
            TargetWidget->RemoveFromViewport();
            bIstargeting = false;
        }
    }
}

bool AWeapon::CheckCanFire()
{
    bool bCanFire = ProjectileCostPerFire <= CurrProjectileNum;
    if (!bCanFire)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ProjectileDrySound, GetActorLocation());
    }
    return bCanFire;
}

void AWeapon::Reload()
{
    if (ReloadSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
    }
    CurrProjectileNum = MaxProjectileNum;
}

void AWeapon::Fire_Implementation(FVector HitLocation)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = Cast<APawn>(GetOwner());

    UWorld* World = GetWorld();

    FVector ProjectileLocation = MuzzleComponent->GetComponentLocation();
    FRotator ProjectileRotation = (HitLocation - ProjectileLocation).Rotation();
    
    CurrProjectileNum = UKismetMathLibrary::FMax(0.f, CurrProjectileNum - ProjectileCostPerFire);

    if (ProjectileClass && GetOwner()->HasAuthority())//仅在server生成子弹，复制到client
    {
        AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, ProjectileLocation, ProjectileRotation, SpawnParams);
        Projectile->SetOwner(GetOwner());
        Projectile->SetInstigator(GetInstigator());
    }
    ++IsSimulating;
    if (GetOwner()->HasAuthority())
    {
        SimulateFire();
    }
}

void AWeapon::SimulateFire_Implementation()
{
    FVector ProjectileLocation = MuzzleComponent->GetComponentLocation();
    ACharacter* MyCharacter = Cast<ACharacter>(GetOwner());
    APlayerController* MyController = Cast<APlayerController>(MyCharacter->GetController());

    if (GunSpark)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GunSpark, ProjectileLocation, ProjectileLocation.Rotation(), 1.0f);
    }
    if (ShellCase)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShellCase, ShellCaseComponent->GetComponentLocation(), ProjectileLocation.Rotation(), 1.0f);
    }
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, ProjectileLocation);
    }
    if (FireMontage && MyCharacter)
    {
        MyCharacter->PlayAnimMontage(FireMontage, FireMontagePlayRate);
    }
    if (FireCameraShake && MyController && MyCharacter->IsLocallyControlled())
    {
        MyController->ClientStartCameraShake(FireCameraShake, 1.f);
    }
    IsSimulating = 0;
}

void AWeapon::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    //扣血
    if (OtherActor != this && OtherActor != GetOwner())
    {
        UAttributeComponent* AttributeComponent = Cast<UAttributeComponent>(OtherActor->GetComponentByClass(UAttributeComponent::StaticClass()));
        if (AttributeComponent)
        {
            AttributeComponent->ApplyHealthChange(GetOwner(), Damage);
        }
    }
    //施加冲量
    if (OtherActor != this && OtherActor != GetOwner() && OtherComponent->IsSimulatingPhysics())
    {
        //OtherComponent->AddImpulseAtLocation(FVector(100.f, 100.f, 100.f), Hit.ImpactPoint);
    }
}

void AWeapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATwinblastCharacter* TargetCharacter = Cast<ATwinblastCharacter>(OtherActor);
    if (TargetCharacter)
    {
        TargetCharacter->AddWeapon(GetClass());
    }
    Destroy();
}

void AWeapon::CloseHit()
{
    TArray<AActor*> IgnoreActor;
    IgnoreActor.Add(GetOwner());
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), 20.f, nullptr, IgnoreActor, this);
}

void AWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AWeapon, IsSimulating);
}


void AWeapon::OnRep_IsSimulating()
{
    if (IsSimulating > 0)
    {
        SimulateFire(); 
    }
}