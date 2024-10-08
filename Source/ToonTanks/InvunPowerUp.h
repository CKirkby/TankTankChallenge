// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InvunPowerUp.generated.h"

class URotatingMovementComponent;
class USphereComponent;

UCLASS()
class TOONTANKS_API AInvunPowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInvunPowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent;
	UPROPERTY(EditDefaultsOnly)
	URotatingMovementComponent* Rotator;

	class ATank* PlayerTank;
	class UHealthComponent* PlayerHealthComp;

	UFUNCTION()
	void OverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* PowerUpSound;
};
