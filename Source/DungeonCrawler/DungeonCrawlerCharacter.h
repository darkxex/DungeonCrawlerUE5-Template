// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "InteractInterface.h"
#include "DungeonCrawlerCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class ADungeonCrawlerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Turn left Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* TurnQAction;
	/** Turn right Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* TurnEAction;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* PressFAction;

	
public:
	ADungeonCrawlerCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;



	

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	void MoveStop(const FInputActionValue& Value);
	void TurnQ();
	void TurnQReleased();
	void TurnE();
	void TurnEReleased();
	void Fbutton();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

    
public:
	/** Returns Mesh1P subobject **/
	UPROPERTY(EditAnywhere, Category = Crawler)
	USphereComponent* SphereComp;
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	USoundCue* propellerAudioCue;
	USoundCue* wallAudioCue;
	USoundCue* buttonAudioCue;
    UAudioComponent* propellerAudioComponent;
	virtual void Tick( float DeltaTime ) override;
	
	bool AtRest();
	bool AtRestRotate();
	UPROPERTY(EditAnywhere, Category = Crawler)
	FVector targetGridPos;
	UPROPERTY(EditAnywhere, Category = Crawler)
	FVector targetPosition;
	UPROPERTY(EditAnywhere, Category = Crawler)
	FVector prevTargetGridPos;
	UPROPERTY(EditAnywhere, Category = Crawler)
	FRotator targetRotation;
	UPROPERTY(EditAnywhere, Category = Crawler)
	bool smoothTransition = false;
	UPROPERTY(EditAnywhere, Category = Crawler)
	float transitionSpeed = 0.1;
	UPROPERTY(EditAnywhere, Category = Crawler)
	float transitionRotationSpeed = 0.1;

	UPROPERTY(EditAnywhere, Category = Crawler)
	float stepGrid = 600;
	void isValidSoundPlay(USoundCue* Sound);
	bool CanRotateQ = true;
	bool CanRotateE = true;
	APlayerController* PController;
 	bool resultCast(FVector direction);
	UPROPERTY(EditDefaultsOnly, Category = Crawler)
	AActor* Prueba;

	UPROPERTY(EditAnywhere, Category = Crawler)
	float WaitTime = 3;
	UPROPERTY(EditAnywhere, Category = Crawler)
	float LerpDuration = 3;
	float TimeElapsed = 0;

	bool canMove = true;
};

