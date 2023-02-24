// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonCrawlerCharacter.h"
#include "DungeonCrawlerProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// ADungeonCrawlerCharacter

ADungeonCrawlerCharacter::ADungeonCrawlerCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	 

    // Load our Sound Cue for the propeller sound we created in the editor... 
    // note your path may be different depending
    // on where you store the asset on disk.
    static ConstructorHelpers::FObjectFinder<USoundCue> propellerCue(
        TEXT("'/Game/Sound/FootStep.FootStep'")
    );

	static ConstructorHelpers::FObjectFinder<USoundCue> wallCue(
        TEXT("'/Game/Sound/WallCol.WallCol'")
    );

    // Store a reference to the Cue asset - we'll need it later.
    propellerAudioCue = propellerCue.Object;
	wallAudioCue = wallCue.Object;
    // Create an audio component, the audio component wraps the Cue, 
    // and allows us to ineract with
    // it, and its parameters from code.
    propellerAudioComponent = CreateDefaultSubobject<UAudioComponent>(
        TEXT("PropellerAudioComp")
    );

    // I don't want the sound playing the moment it's created.
    propellerAudioComponent->bAutoActivate = false;
    // I want the sound to follow the pawn around, so I attach it to the Pawns root.
    propellerAudioComponent->SetupAttachment (RootComponent);
    // I want the sound to come from slighty in front of the pawn.
    propellerAudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));



}

void ADungeonCrawlerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		PController = PlayerController;
		
	}
	targetPosition = FVector (FMath::RoundFromZero(GetActorLocation().X),FMath::RoundFromZero(GetActorLocation().Y),FMath::RoundFromZero(GetActorLocation().Z));
	
	targetGridPos = targetPosition;
}

//////////////////////////////////////////////////////////////////////////// Input

void ADungeonCrawlerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeonCrawlerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ADungeonCrawlerCharacter::MoveStop);
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADungeonCrawlerCharacter::Look);
		
		//Looking
		EnhancedInputComponent->BindAction(TurnQAction, ETriggerEvent::Triggered, this, &ADungeonCrawlerCharacter::TurnQ);
		EnhancedInputComponent->BindAction(TurnQAction, ETriggerEvent::Completed, this, &ADungeonCrawlerCharacter::TurnQReleased);
		//Looking
		EnhancedInputComponent->BindAction(TurnEAction, ETriggerEvent::Triggered, this, &ADungeonCrawlerCharacter::TurnE);
		EnhancedInputComponent->BindAction(TurnEAction, ETriggerEvent::Completed, this, &ADungeonCrawlerCharacter::TurnEReleased);
		
	}
}

void ADungeonCrawlerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float disPosition = FVector::Distance(GetActorLocation(),targetPosition);
	FVector effect = FMath::VInterpConstantTo(GetActorLocation(),targetPosition, DeltaTime, (disPosition / transitionSpeed));
	SetActorLocation(effect);
	//UE_LOG(LogTemp, Display, TEXT("Move Dis %f"),disPosition);
	
	FRotator effectRotate = FMath::RInterpConstantTo(PController->GetControlRotation(),targetRotation, DeltaTime, transitionRotationSpeed);
	PController->SetControlRotation(effectRotate);

}

bool ADungeonCrawlerCharacter::AtRest()
{
	float dotRotate = UKismetMathLibrary::Dot_VectorVector(PController->GetControlRotation().Vector(),targetRotation.Vector());
	
	if ((FVector::Distance(GetActorLocation(),targetGridPos) < 50 ) && dotRotate == 1 )
	{
  		 
		return true;
	}

	else
	{return false;}
	
}

void ADungeonCrawlerCharacter::isValidSoundPlay(USoundCue *sound)
{
	if (sound->IsValidLowLevelFast()) {
		UE_LOG(LogTemp, Display, TEXT("Play Sound"));
       		propellerAudioComponent->SetSound(sound);
			propellerAudioComponent->Play();
			}
}

bool ADungeonCrawlerCharacter::resultCast(FVector direction)
{
FHitResult Hit;
FVector TraceStart = GetActorLocation();
FVector TraceEnd = GetActorLocation() + direction * stepGrid;
FCollisionQueryParams QueryParams;
QueryParams.AddIgnoredActor(this);
GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams);
DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 0.1f, 0, 1.0f);
UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *TraceStart.ToCompactString(), *TraceEnd.ToCompactString());

if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
{
	UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName());
	isValidSoundPlay(wallAudioCue);
	return false;
}
else {
	 return true;
}
   
}

void ADungeonCrawlerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		isMovingzero = false;
		// add movement 
		//AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		if (MovementVector.Y > 0)
		{
			if (AtRest() && resultCast(GetActorForwardVector()) )
			{targetGridPos += (GetActorForwardVector()*stepGrid);
			isValidSoundPlay(propellerAudioCue);
			}
		}
		if (MovementVector.Y < 0)
		{
			if (AtRest() && resultCast(-GetActorForwardVector()))
			{targetGridPos -= (GetActorForwardVector()*stepGrid);
			isValidSoundPlay(propellerAudioCue);
			}
		}

		if (MovementVector.X > 0)
		{
			if (AtRest()&& resultCast(GetActorRightVector()))
			{targetGridPos += (GetActorRightVector()*stepGrid);
			isValidSoundPlay(propellerAudioCue);
			}
		}
		if (MovementVector.X < 0)
		{
			if (AtRest()&& resultCast(-GetActorRightVector()))
			{targetGridPos -= (GetActorRightVector()*stepGrid);
			isValidSoundPlay(propellerAudioCue);
			}
		}
		
		
		prevTargetGridPos = targetGridPos;
		targetPosition = targetGridPos;

		

	UE_LOG(LogTemp, Display, TEXT("This value %s"), *targetPosition.ToCompactString());

	}
}

void ADungeonCrawlerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		//AddControllerYawInput(LookAxisVector.X);
		//AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADungeonCrawlerCharacter::MoveStop(const FInputActionValue &Value)
{
	isMovingzero = true;
	UE_LOG(LogTemp, Display, TEXT("Stop"));
}

void ADungeonCrawlerCharacter::TurnQ()
{
	if (CanRotateQ == true && isMovingzero == true)
	{   SetActorLocation(targetPosition);
		UE_LOG(LogTemp, Display, TEXT("Turn Q"));

	if (AtRest())
	{	
		UE_LOG(LogTemp, Display, TEXT("AtRest OK"));
		targetRotation -= FRotator(0,1,0) * 90;
		isValidSoundPlay(propellerAudioCue);
	
	}else
	UE_LOG(LogTemp, Display, TEXT("AtRest Fail"));

	if (targetRotation.Yaw > 270 && targetRotation.Yaw < 361) targetRotation.Yaw = 0;
	if (targetRotation.Yaw < 0) targetRotation.Yaw = 270;
	CanRotateQ = false;
	}
	
		
}

void ADungeonCrawlerCharacter::TurnQReleased()
{
	CanRotateQ = true;
}

void ADungeonCrawlerCharacter::TurnE()
{	if (CanRotateE == true )
{	SetActorLocation(targetPosition);
	UE_LOG(LogTemp, Display, TEXT("Turn E"));
	if (AtRest())
	{	
		UE_LOG(LogTemp, Display, TEXT("AtRest OK"));
		targetRotation += FRotator(0,1,0) * 90;
		isValidSoundPlay(propellerAudioCue);
	
	}else
	UE_LOG(LogTemp, Display, TEXT("AtRest Fail"));
	if (targetRotation.Yaw > 270 && targetRotation.Yaw < 361) targetRotation.Yaw = 0;
	if (targetRotation.Yaw < 0) targetRotation.Yaw = 270;
	CanRotateE = false;
	
	}
}

void ADungeonCrawlerCharacter::TurnEReleased()
{
	CanRotateE = true;
}

void ADungeonCrawlerCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ADungeonCrawlerCharacter::GetHasRifle()
{
	return bHasRifle;
}
