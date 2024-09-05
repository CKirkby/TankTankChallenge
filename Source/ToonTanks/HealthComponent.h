#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


class ATank;
class USphereComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsInvunerable = false;
	
	void SetInvunerable();


private:
	UPROPERTY(EditAnywhere)
	float MaxHeath = 100.f;
	float Health = 0.f;

	class AToonTanksGameMode* ToonTanksGameMode;

	ATank* Tank;

	// Creates a function ready to access the damage taken 
	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser);

	FTimerHandle InvunTimerHandle;
	UPROPERTY(EditAnywhere)
	float InvunTime = 10.f;
	void EndInvun();
};
