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
}


// Called when the game starts

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();

	SetupInputComponent();
}


// Called every frame

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());	
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a 'Physics Handle' Component attached! Please add one in the blueprint class"), *GetOwner()->GetName());		
	}
}

FVector UGrabber::GetLineTraceEnd()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	FVector Location;
	FRotator Rotation;

	PC->GetPlayerViewPoint(OUT Location, OUT Rotation);

	return Location + (Rotation.Vector() * Reach);

	
}

FHitResult UGrabber::GetFirstPhysicsActorInReach() const
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	FVector Location;
	FRotator Rotation;

	PC->GetPlayerViewPoint(OUT Location, OUT Rotation);

	FVector LineTraceEnd = Location + (Rotation.Vector() * Reach);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	Params.AddIgnoredActor(GetOwner());
	bool HasHit = GetWorld()->LineTraceSingleByChannel(Hit, Location, LineTraceEnd, ECC_PhysicsBody, Params );

	if (HasHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has hit %s"), *Hit.Actor->GetName());
	}

	return Hit;
	
	
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Interact", IE_Pressed, this, &UGrabber::PickUpObject);
		InputComponent->BindAction("Interact", IE_Released, this, &UGrabber::ReleaseObject);
	}
}

void UGrabber::PickUpObject()
{
	FHitResult Hit = GetFirstPhysicsActorInReach();

	AActor* HitActor = Hit.GetActor();
	
	if (HitActor)
	{
		UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocation( ComponentToGrab, NAME_None, Hit.Location);
	}

	
}

void UGrabber::ReleaseObject()
{
	if (!PhysicsHandle) { return; }
	
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
	
}