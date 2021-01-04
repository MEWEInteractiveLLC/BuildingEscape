// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"


#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Grabber Settings")
	float Reach = 100.0f;
	
	// Called when the game starts
	virtual void BeginPlay() override;


private:


	class UPhysicsHandleComponent* PhysicsHandle = nullptr;

	class UInputComponent* InputComponent = nullptr;

	bool bIsTryingToInteract = false;




protected:

	void SetupInputComponent();
	
	void FindPhysicsHandle();

	FHitResult GetFirstPhysicsActorInReach() const;

	void PickUpObject();

	void ReleaseObject();
	
};




