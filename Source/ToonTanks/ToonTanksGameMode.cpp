// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Tank.h"
#include "ToonTanksPlayerController.h"
#include "Tower.h"
#include "kismet/GameplayStatics.h"

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();

	SpawnTurret();
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
			GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		}
		GameOver(false);
	}
	else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
		CurrentTowers --;
		UE_LOG(LogTemp, Display, TEXT("Turrets: %i"), CurrentTowers);
		if(CurrentTowers == 0)
		{
			SpawnTurret();
			//GameOver(true);
		}
	}
}

void AToonTanksGameMode::HandleGameStart()
{
	CurrentTowers = GetTargetTowerCount();
	
	// Gets references to the tank and controller by casting
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();
	
	if (ToonTanksPlayerController)
	{
		// Sets the input as false to start.
		ToonTanksPlayerController->SetPlayerEnabledState(false);

		// Creates a delay (timer) that when complete, calls the start enabled state and passes in true
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false);
	}

	if(TowerSpawnTimerActivate == true)
	{
		// Creates a timer to start spawning the towers
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AToonTanksGameMode::SpawnTurret, TowerSpawnTimer, true);
	}
}

FVector AToonTanksGameMode::CalculateSpawnArea(AActor* OriginActor, float MinDistance, float MaxDistance)
{
	// Calculates a random angle in radians (is 360 degrees)
	float RandomAngle = FMath::RandRange(0.f, 2.0f * PI);
	float RandomDistance = FMath::RandRange(MinDistance, MaxDistance);

	// Calculates a random offset in the x and y vectors by multiplying a cos and sin angle
	float OffsetX = RandomDistance * FMath::Cos(RandomAngle);
	float OffsetY =  RandomDistance * FMath::Sin(RandomAngle);

	// Manually sets the map bounds
	FVector2d MinMapBoundary = FVector2d(-2350.f, -2350.f);
	FVector2d MaxMapBoundary = FVector2d(2350.f, 2350.f);

	// Adds all the random variables together to get a random location within a radius and returns it 
	RandomLocation = OriginActor->GetActorLocation() + FVector(OffsetX, OffsetY, 0);

	/* If this is correct, should check if any parts of the spawn location are greater than the boundary I have set, if so it will clamp it to the boundary max
	   and place it back into the level albeit at the edge rather then random*/
	if (RandomLocation.X < MinMapBoundary.X || RandomLocation.X > MaxMapBoundary.X || RandomLocation.Y < MinMapBoundary.Y || RandomLocation.Y > MaxMapBoundary.Y)
	{
		RandomLocation.X = FMath::Clamp(RandomLocation.X, MinMapBoundary.X, MaxMapBoundary.X);
		RandomLocation.Y = FMath::Clamp(RandomLocation.Y, MinMapBoundary.Y, MaxMapBoundary.Y);
	}
	
	return RandomLocation;
}

void AToonTanksGameMode::SpawnTurret()
{
	// Gets the location from the random calculated location
	FVector SpawnLocation = CalculateSpawnArea(Tank, MinRadius, MaxRadius);
	
	// Checks the tower is not null, there is not more than needed and the activation bool is true.
	if (TowerClass != nullptr && CurrentTowers < MaxTowers && TowerSpawnTimerActivate == true)
	{
		GetWorld()->SpawnActor<ATower>(TowerClass, SpawnLocation, FRotator::ZeroRotator);
		CurrentTowers++;
	}
	UE_LOG(LogTemp, Display, TEXT("Turrets: %i"), CurrentTowers);
}

int32 AToonTanksGameMode::GetTargetTowerCount() const
{
	TArray<AActor*> TowerArray;
	
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), TowerArray);

	return TowerArray.Num();
}



