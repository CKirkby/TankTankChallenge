// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	void ActorDied(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

#pragma region Tower Spawn
	// Tower Spawn variables and functions
	UPROPERTY(EditAnywhere, Category = "Tower Spawn")
	TSubclassOf<class ATower> TowerClass;
	UPROPERTY()
	class ATower* Tower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	bool TowerSpawnTimerActivate = true;

	// Tower radius variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	float MinRadius = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	float MaxRadius = 2000.f;
	UPROPERTY(EditAnywhere, Category = "Tower Spawn")
	bool DebugSpheres = false;
	FVector RandomLocation;

	// Tower spawn boundary variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	FVector MinMapBoundary = FVector(-2350.f, -2350.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	FVector MaxMapBoundary = FVector(2350.f, 2350.f, 0.f);

	// Tower Count variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	int32 MaxTowers = 6;
	int32 CurrentTowers = 0;

	// Timer delay 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	float TowerSpawnTimer = 15.f;

	// Functions for 
	UFUNCTION(BlueprintCallable)
	void SpawnTurret();
	FVector CalculateSpawnArea(AActor* Origin, float MinDistance, float MaxDistance);
#pragma endregion
	
private:
	class ATank* Tank;

	class AToonTanksPlayerController* ToonTanksPlayerController;

	float StartDelay = 3.f;

	void HandleGameStart();
	
	int32 GetTargetTowerCount() const;

	FTimerHandle SpawnTimerHandle;
};