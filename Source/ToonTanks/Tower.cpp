#include "Tower.h"
#include "Tank.h"
#include "TimerManager.h"
#include "kismet/GameplayStatics.h"


void ATower::BeginPlay()
{
	Super::BeginPlay();

	// Casts to the tank player character and gets a reference.
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	// Initialises the timer for the fire function by the fire rate time and true to loop.
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (InFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

bool ATower::InFireRange()
{
	if (Tank != nullptr)
	{
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		if (Distance <= FireRange)
		{
			return true;
		}
	}
	return false;
}

void ATower::CheckFireCondition()
{
	if (Tank == nullptr)
	{
		return;
	}
	if (InFireRange() && Tank->bAlive == true)
	{
		Fire();
	}
}

