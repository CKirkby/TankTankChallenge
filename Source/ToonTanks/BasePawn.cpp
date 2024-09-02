#include "BasePawn.h"

#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Capsule Construction
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider")); // Constructs a capsule component and stores the address in the capsule component ref
	RootComponent = CapsuleComp; // Makes the capsule comp the root.

	// Static Mesh Construction
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp); // Attaches the comp to the root comp (capsule at this point)

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh); // Attaches to the base mesh.

	// Scene Component Construction
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projetile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh); // Attaches to the turret mesh
} 

void ABasePawn::HandleDestruction() const
{
	if (DeathParticles != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}
	if (DeathSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
	if (DeathCameraShakeClass != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0, ToTarget.Rotation().Yaw, 0);
	TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 5));
}

void ABasePawn::Fire()
{
	// Spawns the projectile actor on the projectile spawn point.
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	Projectile->SetOwner(this);
}




