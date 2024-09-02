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

	// Tower Spawn vairables and functions
	UPROPERTY(EditAnywhere, Category = "Tower Spawn")
	TSubclassOf<class ATower> TowerClass;
	UPROPERTY()
	class ATower* Tower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	bool TowerSpawnTimerActivate = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	float MinRadius = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	float MaxRadius = 2000.f;
	FVector RandomLocation;

	FVector MinBoundsInput = FVector(-2450.000000,-2460.000000,70.062599);
	FVector MaxBoundsInput = FVector(2450.000000,2460.000000,70.062599);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	int32 MaxTowers = 6;
	int32 CurrentTowers = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Spawn")
	float TowerSpawnTimer = 15.f;
	
	UFUNCTION(BlueprintCallable)
	void SpawnTurret();
	FVector CalculateSpawnArea(AActor* Origin, float MinDistance, float MaxDistance);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);
	
private:
	class ATank* Tank;

	class AToonTanksPlayerController* ToonTanksPlayerController;

	float StartDelay = 3.f;

	void HandleGameStart();
	
	int32 GetTargetTowerCount() const;

	FTimerHandle SpawnTimerHandle;
};