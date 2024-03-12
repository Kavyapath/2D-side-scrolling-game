// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player1.generated.h"

class UAnimMontage;

UCLASS()
class TESST_ALIENDOG_API APlayer1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMax = "100", ClampMin = "0", UIMax = "100", UIMin = "0"))
		float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float Health = MaxHealth;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float FallHeight = 700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float FallDamage = 30.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Life")
		float Points = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Life")
		float Life = 0;

	bool IsDead() const;

private:
	void MoveForward(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);
	void MoveRight(float AxisValue);
	void ToggleGliding();
	void StartGliding();
	void StopGliding();
	bool CanStartgliding();
	void RecordOriginalSettings();
	void ApplyOriginalSetting();
	void DescentPlayer();
	void DamageOnFall();
	virtual void Landed(const FHitResult& Hit) override;
	

	

	

	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	UPROPERTY(EditAnywhere)
		float RotationRate = 100;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCameraComponent* Camera;//forward declare them

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* GliderMesh;

	//Anim montages
	UPROPERTY(EditDefaultsOnly, Category = "Montagess")
	UAnimMontage* AttackMontage;



	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Paraglider", meta = (AllowPrivateAccess = "true"))
		bool bIsGliding = false;
	UPROPERTY(EditAnywhere)
		FVector CurrentVelocity;
	UPROPERTY(EditAnywhere)
		float DescendingRate = 300.f;




	float WorldDeltaTime;
	bool OriginalOrientRotation;
	float OriginalGravityScale;
	float OriginalWalkingSpeed;
	float OriginalDeceleration;
	float OriginalAcceleration;
	float OriginalAirControl;
	bool OriginalDesiredRotation;
	float MinimumHeight = 300.f;

};
