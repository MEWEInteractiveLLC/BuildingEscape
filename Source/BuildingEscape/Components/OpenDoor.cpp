// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw = InitialYaw + 90.f;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Does not have a Pressure plate assigned"), *GetOwner()->GetName());
	}

	FindAudioComponent();

	

	
	

	// ...
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActors()> MassToOpen)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
	
		
	

	

	

	// ...
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	
	CloseDoorSound = false;
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, OpeningSpeed);
	FRotator FinalRot(0.f, 0.0f, 0.f);
	FinalRot.Yaw = CurrentYaw;
	GetOwner()->SetActorRelativeRotation(FinalRot);

	if (!AudioComponent) { return; }
	if (!OpenDoorSound)
	{
		OpenDoorSound = true;
		AudioComponent->Play(0.0f);
	
	}
	
	
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, ClosingSpeed);
	FRotator FinalRot(0.f, 0.0f, 0.f);
	FinalRot.Yaw = CurrentYaw;
	GetOwner()->SetActorRelativeRotation(FinalRot);
	
	DoorLastOpened = 0.0f;

	if (!AudioComponent) { return; }
	if (!CloseDoorSound)
	{
		CloseDoorSound = true;
		OpenDoorSound = false;
		AudioComponent->Play(0.0f);
	}

	
	
	
}

float UOpenDoor::GetTotalMassOfActors() const
{
	float TotalMass = 0.0f;

	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { return 0.0f; }

	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (auto Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Total mass is %f"), TotalMass);
	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing audio component"), *GetOwner()->GetName());
		return;
	}
}

