// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabBehaviour.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Engine/SkeletalMeshSocket.h"


#define out
// Sets default values for this component's properties
UGrabBehaviour::UGrabBehaviour()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabBehaviour::BeginPlay()
{
	Super::BeginPlay();
	
	
	// ...
	UInputComponent* inputComponent = this->GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent != nullptr) {
		inputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabBehaviour::Grab);
		inputComponent->BindAction("Release", EInputEvent::IE_Pressed, this, &UGrabBehaviour::Release);
		inputComponent->BindAction("Throw", EInputEvent::IE_Pressed, this, &UGrabBehaviour::Throw);
	}
	
}


// Called every frame
void UGrabBehaviour::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	;FName name = "Offset Socket";
	// ...
	if (hasGrabbed) {
		FVector location;
		FRotator rotation;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(out location, out rotation);
		FVector lineTracedEnd = location + rotation.Vector() * REACH;

		if (grabbedActor != nullptr) {

			if (physicsHandle != nullptr) {
				physicsHandle->SetTargetLocation(lineTracedEnd);
			}
			else {
				FVector grabbedLoc = grabbedActor->GetActorLocation();
				grabbedLoc = lineTracedEnd;
				grabbedActor->SetActorLocation(grabbedLoc);
			}
		}
	}
}

void UGrabBehaviour::Grab()
{
	FVector location;
	FRotator rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(out location, out rotation);
	FVector lineTracedEnd = location + rotation.Vector() * REACH;
	FVector lineTracedLoc = location + rotation.Vector() * REACH;

	DrawDebugLine(GetWorld(), location, lineTracedEnd, FColor::Blue, false, 1.0f, 0, 5.0f);

	FHitResult hit;
	FCollisionQueryParams traceParams(FName(TEXT("traceQuery")), false, GetOwner()); // parameters for the collisions
	FCollisionObjectQueryParams objectTypeParams(ECollisionChannel::ECC_PhysicsBody); // Objects you want to get hit, ECC_PhysicsBody = all physics objects
	GetWorld()->LineTraceSingleByObjectType(hit, location, lineTracedEnd, objectTypeParams, traceParams);

	grabbedActor = hit.GetActor();
	if (grabbedActor != nullptr) {
		hasGrabbed = true;
		primitiveComp = hit.GetComponent();
		//UE_LOG(LogTemp, Warning, TEXT("Grabbed %s"), *grabbedActor->GetName());

		physicsHandle = grabbedActor->FindComponentByClass<UPhysicsHandleComponent>();
		if (physicsHandle != nullptr) {
			physicsHandle->GrabComponentAtLocation(primitiveComp, EName::None, lineTracedEnd);
		}
	
	}

}

void UGrabBehaviour::Release()
{
	hasGrabbed = false;
	grabbedActor = nullptr;

	if (physicsHandle != nullptr) {
		physicsHandle->ReleaseComponent();
	}
}


void UGrabBehaviour::Throw()
{
	
}

