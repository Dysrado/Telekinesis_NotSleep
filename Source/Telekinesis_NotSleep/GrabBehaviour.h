// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabBehaviour.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TELEKINESIS_NOTSLEEP_API UGrabBehaviour : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabBehaviour();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(EditAnywhere) float REACH = 600;
	AActor* grabbedActor = NULL;
	bool hasGrabbed = false;
	UPrimitiveComponent* primitiveComp;
	UPhysicsHandleComponent* physicsHandle = NULL;
	void Grab();
	void Release();
	void Throw();

	//UPROPERTY(EditAnywhere) AActor* Offset;
	USkeletalMeshComponent* mesh;
	TArray<USkeletalMeshComponent*> meshList;
	TArray<AActor*> actorList;
	UPROPERTY(EditAnywhere) AActor* offset;
		
};
