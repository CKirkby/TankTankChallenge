#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	void HandleDestruction();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	class ATank* Tank;

	UPROPERTY(EditAnywhere, Category = "Combat Parameters")
	float FireRange = 1000.f;

	FTimerHandle FireRateTimerHandle;
	float FireRate = 2.f;
	void CheckFireCondition();

	bool InFireRange();
};
