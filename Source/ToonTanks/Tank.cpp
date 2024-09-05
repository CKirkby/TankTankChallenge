// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"

// Constructor
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = true;

	// Construct the camera components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	ShieldSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield Sphere"));
	ShieldSphere->SetupAttachment(RootComponent);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Binds the functions to the input mappings.
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());
	
	DisableShields();
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		// Creates a line trace for the mouse
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25, 12, FColor::Emerald, false, -1);

		RotateTurret(HitResult.ImpactPoint);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	// Sets the actor to be hidden and disables tick
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

void ATank::Move(float Value)
{
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value * MovementSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float TurnValue)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = TurnValue * TurnSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::EnableShields() const
{
	if(ShieldSphere)
	{
		ShieldSphere->SetVisibility(true);
		UE_LOG(LogTemp, Display, TEXT("Deavtivated Shields"));
	}
}

void ATank::DisableShields() const
{
	if(ShieldSphere)
	{
		ShieldSphere->SetVisibility(false);
		UE_LOG(LogTemp, Display, TEXT("Activated Shields"));
	}
}


