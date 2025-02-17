// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

#pragma region Private Variable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category="Door Settings")
	float TargetYaw = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category="Door Settings")
	float OpeningSpeed = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category="Door Settings")
	float ClosingSpeed = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category="Door Settings")
	float DoorCloseDelay = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category="Door Settings")
	float MassToOpen = 50.0f;

	float InitialYaw;
	float CurrentYaw;

	UPROPERTY()
	class UAudioComponent* AudioComponent = nullptr;

	
	float DoorLastOpened = 0.0f;
#pragma endregion
	



	

#pragma region Private Functions

	void OpenDoor(float DeltaTime);

	void CloseDoor(float DeltaTime);

	float GetTotalMassOfActors() const;

	void FindAudioComponent();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;
	
#pragma endregion 


public:

#pragma region Reference Variables
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ATriggerVolume* PressurePlate;
	
#pragma endregion

#pragma region Public Functions
	
#pragma endregion

		
};
