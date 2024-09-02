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
	float RandomAngle = FMath::RandRange(0.f, 2.0f * PI);
	float RandomDistance = FMath::RandRange(MinDistance, MaxDistance);
        
	float OffsetX = RandomDistance * FMath::Cos(RandomAngle);
	float OffsetY =  RandomDistance * FMath::Sin(RandomAngle);
	RandomLocation = OriginActor->GetActorLocation() + FVector(OffsetX, OffsetY, 0);

	return RandomLocation;
}

void AToonTanksGameMode::SpawnTurret()
{
	FVector SpawnLocation = CalculateSpawnArea(Tank, MinRadius, MaxRadius);
		
	// Checks if the bool has been set to false
	if (TowerSpawnTimerActivate == false)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}

	// Checks the tower is not a null and spawns.
	if (TowerClass != nullptr && CurrentTowers < MaxTowers)
	{
		if(TowerSpawnTimerActivate == true)
		{
			GetWorld()->SpawnActor<ATower>(TowerClass, SpawnLocation, FRotator::ZeroRotator);
			CurrentTowers++;
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Turrets: %i"), CurrentTowers);
}

int32 AToonTanksGameMode::GetTargetTowerCount() const
{
	TArray<AActor*> TowerArray;
	
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), TowerArray);

	return TowerArray.Num();
}



