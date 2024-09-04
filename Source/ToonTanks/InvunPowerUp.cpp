// Fill out your copyright notice in the Description page of Project Settings.


#include "InvunPowerUp.h"

#include "HealthComponent.h"
#include "Tank.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInvunPowerUp::AInvunPowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Sets up the primary components
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Main Collision"));
	RootComponent = SphereComponent;

	Rotator = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotator"));

	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AInvunPowerUp::OverlapEvent);
}

// Called when the game starts or when spawned
void AInvunPowerUp::BeginPlay()
{
	Super::BeginPlay();

	// Gets the player tank and the health component.
	PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerHealthComp = PlayerTank->FindComponentByClass<UHealthComponent>();
}

// Called every frame
void AInvunPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInvunPowerUp::OverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		PlayerHealthComp->SetInvunerable();
		UGameplayStatics::PlaySoundAtLocation(this, PowerUpSound, GetActorLocation());
		UE_LOG(LogTemp, Display, TEXT("Pickup Picked Up"));
		Destroy();
	}
}


