// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverDoor.h"
#include "Door.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ALeverDoor::ALeverDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshLever = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SphereComp->SetupAttachment(MeshLever);
    SphereComp->SetSphereRadius(200);
	static ConstructorHelpers::FObjectFinder<USoundWave> HitSoundAsset(TEXT("'/Game/Sound/button-17.button-17'"));
    if (HitSoundAsset.Succeeded())
        HitSound = HitSoundAsset.Object;
}

// Called when the game starts or when spawned
void ALeverDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALeverDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeverDoor::OnInteract()
{
	//play sound
		 if (HitSound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
    }
	//endplaysound

	if (LeverisPressed == false)
	{FVector newLocation = GetActorLocation() - FVector(28,0,0);
		SetActorLocation(newLocation);
		LeverisPressed = true;
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Blue, FString::Printf(TEXT("You have pressed the button...")));
		 
		 
for (TObjectIterator<ADoor> Itr; Itr; ++Itr)
{
	ADoor* Actor = *Itr;
	 if (Actor->ActorHasTag(FName(TEXT("bpdoor"))))
	{
		Actor->canOpen = true;
	} 
	else
	{
		continue;
	}
}


		}
		else
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("The button has already been pressed...")));
}
