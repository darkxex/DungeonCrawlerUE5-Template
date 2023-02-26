// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "LeverDoor.generated.h"

UCLASS()
class DUNGEONCRAWLER_API ALeverDoor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeverDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* MeshLever;
	UPROPERTY(EditAnywhere, Category = Crawler)
	USphereComponent* SphereComp;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
    virtual void OnInteract();

	bool LeverisPressed = false;
};
