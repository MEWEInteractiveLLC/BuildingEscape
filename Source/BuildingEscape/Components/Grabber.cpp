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

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	FVector Location;
	FRotator Rotation;

	PC->GetPlayerViewPoint(OUT Location, OUT Rotation);

	FVector LineTraceEnd = Location + (Rotation.Vector() * Reach);

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);	
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a 'Physics Handle' Component attached! Please add one in the blueprint class"), *GetOwner()->GetName());		
	}
}

FHitResult UGrabber::GetFirstPhysicsActorInReach() const
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	FVector Location;
	FRotator Rotation;

	PC->GetPlayerViewPoint(OUT Location, OUT Rotation);

	FVector LineTraceEnd = Location + (Rotation.Vector() * Reach);

	

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

	if (Hit.GetActor())
	{
		UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();

		PhysicsHandle->GrabComponentAtLocation( ComponentToGrab, NAME_None, Hit.Location);
	}

	
}

void UGrabber::ReleaseObject()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
	
}