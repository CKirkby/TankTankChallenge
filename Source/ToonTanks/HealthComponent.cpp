// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHeath;

	// Binds the damage taken function to on take any damage
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);

	ToonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;

	if (IsInvunerable == false)
	{
		Health -= Damage;
	}
	
	if(Health <= 0.f && ToonTanksGameMode)
	{
		ToonTanksGameMode->ActorDied(DamagedActor);
	}
}

void UHealthComponent::SetInvunerable()
{
	IsInvunerable = true;

	GetWorld()->GetTimerManager().SetTimer(InvunTimerHandle, this, &UHealthComponent::EndInvun, InvunTime, false);

	Tank->EnableShields();
}

void UHealthComponent::EndInvun()
{
	IsInvunerable = false;
	Tank->DisableShields();
}



