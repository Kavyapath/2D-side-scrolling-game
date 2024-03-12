// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1_PlayerController.h"
#include "Player1.h"
#include "Kismet/GameplayStatics.h"



void APlayer1_PlayerController::BeginPlay() {
	 Player = Cast<APlayer1>(UGameplayStatics::GetPlayerCharacter(this, 0));
}
void APlayer1_PlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	if (bIsWinner == false) {
		
		if (Player->IsDead() && Player->Life > 0) {
			GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
		}
	}
}