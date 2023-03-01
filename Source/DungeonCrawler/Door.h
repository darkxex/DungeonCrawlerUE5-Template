// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class DUNGEONCRAWLER_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = Crawler)
	FVector StartLocation;
	UPROPERTY(EditAnywhere, Category = Crawler)
	float moved = 700;
	UPROPERTY(EditAnywhere, Category = Crawler)
	FVector currentLocation;
	UPROPERTY(EditAnywhere, Category = Crawler)
	FVector Speed = FVector(0,0,-400);
	UPROPERTY(EditAnywhere, Category = Crawler)
	float dist; 
	UPROPERTY(BlueprintReadWrite, Category = Crawler)
	bool canOpen;
};
