// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabBehaviour.h"
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"
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
	//GetOwner()->GetAttachedActors(actorList);
	GetOwner()->GetAllChildActors(actorList);
	GetOwner()->GetComponents<USkeletalMeshComponent>(meshList);
	if (meshList.Num() > 0)
	{
		
		UE_LOG(LogTemp,Display, TEXT("MESH OFFSET"));
	}
	if (actorList.Num() > 0)
	{
		
		UE_LOG(LogTemp,Display, TEXT("ACTOR OFFSET"));
	}
	mesh = meshList[0];
	
	// ...
	UInputComponent* inputComponent = this->GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent != nullptr) {
		inputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabBehaviour::Grab);
		inputComponent->BindAction("Release", EInputEvent::IE_Pressed, this, &UGrabBehaviour::Release);
		inputComponent->BindAction("Throw", EInputEvent::IE_Pressed, this, &UGrabBehaviour::Throw);
	}
	GrabDelayTime = GrabDelay;
}


// Called every frame
void UGrabBehaviour::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	if (hasGrabbed) {
		FVector location;
		FRotator rotation;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(out location, out rotation);
		FVector lineTracedEnd = location + rotation.Vector() * REACH;

		if (grabbedActor != nullptr) {

			if (physicsHandle != nullptr) {
				
				if (CanGrab) {
					physicsHandle->SetTargetLocation(mesh->GetSocketLocation(name));
				}
				else {
					if (GrabDelayTime <= 0.0f) {
						GrabDelayTime = GrabDelay;
						CanGrab = true;
					}
					else {
						GrabDelayTime -= DeltaTime;
						physicsHandle->SetTargetLocation(lineTracedEnd + FVector(0,0,100));

					}
				}
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
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("MISSED!"));
		grabbedActor = GrabNearestPhysicsObject();
		this->hasGrabbed = true;

		
		primitiveComp = grabbedActor->FindComponentByClass<UPrimitiveComponent>();
		if (primitiveComp != nullptr)
		{
			this->physicsHandle = this->grabbedActor->FindComponentByClass<UPhysicsHandleComponent>();
			if (this->physicsHandle != NULL) {
				//lineTracedEnd = location + rotation.Vector();
				this->physicsHandle->GrabComponentAtLocation(this->primitiveComp, EName::None, grabbedActor->GetActorLocation());
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Null Error"));
		}
		
	}
	CanGrab = false;
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

AActor* UGrabBehaviour::GrabNearestPhysicsObject()
{
	UWorld* world = GetWorld();
	AActor* closestActor = NULL;
	float closestDistance = 10000.0f;

	for (TActorIterator<AActor> It(world, AActor::StaticClass()); It; ++It)
	{
		AActor* actor = *It;
		if (actor != NULL)
		{
			//Targets.Add(actor);
			//UE_LOG(LogTemp, Warning, TEXT("Added Target"));
			physicsHandle = actor->FindComponentByClass<UPhysicsHandleComponent>();
			if (this->physicsHandle != NULL) {
				float distance = (GetOwner()->GetActorLocation() - actor->GetActorLocation()).Size();
				UE_LOG(LogTemp, Display, TEXT("Distance! %f"), distance);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closestActor = actor;
				}
			}
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Closest Distance! %f"), closestDistance);
	return closestActor;
}

