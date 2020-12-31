// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Interact", IE_Pressed, this, &UGrabber::InteractWithObject);
	}

	
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a 'Physics Handle' Component attached! Please add one in the blueprint class"), *GetOwner()->GetName());		
	}

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	FVector Location;
	FRotator Rotation;

	PC->GetPlayerViewPoint(OUT Location, OUT Rotation);

	FVector LineTraceEnd = Location + (Rotation.Vector() * Reach);

	DrawDebugLine(GetWorld(), Location, LineTraceEnd, FColor::Red, false, 0.1f, 0, 2.0f);

	//UE_LOG(LogTemp, Warning, TEXT("The Location Is %s, and the Rotation is %s"), *Location.ToString(), *Rotation.ToString());

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	Params.AddIgnoredActor(GetOwner());
	bool HasHit = GetWorld()->LineTraceSingleByChannel(Hit, Location, LineTraceEnd, ECC_PhysicsBody, Params );

	if (HasHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has hit %s"), *Hit.Actor->GetName());
	}

	// ...
}

void UGrabber::InteractWithObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting With Object"));
}

