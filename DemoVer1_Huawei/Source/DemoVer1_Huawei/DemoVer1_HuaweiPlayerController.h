// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DemoVer1_HuaweiPlayerController.generated.h"

UCLASS()
class ADemoVer1_HuaweiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADemoVer1_HuaweiPlayerController();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetMoveToMouseCursor(bool ToSet);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetIsInConversation(bool ToSet);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetIsKeyboardControl(bool ToSet);

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	/** True if the controlled character should navigate to the mouse cursor. */
	bool bIsCameraRotating = false;

	/** True if the controlled character should navigate to the mouse cursor. */
	bool bInConversation = false;

	/** True if the controlled character should navigate to the mouse cursor. */
	bool bIsKeyboardControl = false;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};


