// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Player1_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESST_ALIENDOG_API APlayer1_PlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	UPROPERTY(EditAnywhere)
		float RestartDelay = 5.f;

	FTimerHandle RestartTimer;

private:


	class APlayer1* Player;
};
