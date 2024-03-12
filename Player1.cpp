// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include"Kismet/KismetMathLibrary.h"


// Sets default values
APlayer1::APlayer1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	GliderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Glider Mesh"));
	GliderMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayer1::BeginPlay()
{
	Super::BeginPlay();
	GliderMesh->SetVisibility(false);
	

}

// Called every frame
void APlayer1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	WorldDeltaTime = DeltaTime;
	DescentPlayer();
	if (GetCharacterMovement()->IsWalking() && bIsGliding) {
		StopGliding();
	}

}

// Called to bind functionality to input
void APlayer1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayer1::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayer1::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayer1::LookUp);//PlayerInputComponent->BindAxis(TEXT("LookUp"), this, AShooterCharacter::LookUp);//this addcontrollerpitch input is define in the pawn class which is a parent class of the character so we can directly call from this also
	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayer1::MoveRight);
	//PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APlayer1::LookRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &APlayer1::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &APlayer1::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);//this jump is define in the character class
	PlayerInputComponent->BindAction(TEXT("Glide"), EInputEvent::IE_Pressed, this, &APlayer1::ToggleGliding);

}


void APlayer1::MoveForward(float AxisValue) {
	//
	//to find which way is forward
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, AxisValue);
}



void APlayer1::MoveRight(float AxisValue) {
	//find out which way is right
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, AxisValue);
}

void  APlayer1::LookUpRate(float AxisValue) {
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayer1::LookRightRate(float AxisValue) {
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}
void APlayer1::LookUp(float AxisValue) {
	AddControllerPitchInput(AxisValue);
}
void APlayer1::LookRight(float AxisValue) {
	AddControllerYawInput(AxisValue);
}

void APlayer1::ToggleGliding()
{
	if (bIsGliding == false) {
		StartGliding();
	}
	else {
		StopGliding();
	}
}

void APlayer1::StartGliding()
{
	if (CanStartgliding()) {
		GliderMesh->SetVisibility(true);

		CurrentVelocity = GetCharacterMovement()->Velocity;
		bIsGliding = true;
		RecordOriginalSettings();
		GetCharacterMovement()->RotationRate = FRotator(0.f, 250.f, 0.f);
		GetCharacterMovement()->GravityScale = 0.f;
		GetCharacterMovement()->AirControl = 0.9f;
		GetCharacterMovement()->BrakingDecelerationFalling = 350.f;
		GetCharacterMovement()->MaxAcceleration = 1024;
		GetCharacterMovement()->MaxWalkSpeed = 600;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;

	}
}



void APlayer1::StopGliding()
{
	ApplyOriginalSetting();
	bIsGliding = false;
	GliderMesh->SetVisibility(false);
}

bool APlayer1::CanStartgliding()
{
	FHitResult Hit;
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = GetActorLocation() + GetActorUpVector() * MinimumHeight * -1.f;
	//now we have to set the collision settings
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	TEnumAsByte<ECollisionChannel> TraceProperties = ECC_Visibility;
	GetWorld()->LineTraceSingleByChannel(Hit,TraceStart,TraceEnd,TraceProperties,QueryParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red);
	if (Hit.bBlockingHit == false && GetCharacterMovement()->IsFalling() == true) 
	{
		return true;
	}
	else
	{
		return false;
	}



}

void APlayer1::RecordOriginalSettings()
{
	 OriginalOrientRotation= GetCharacterMovement()->bOrientRotationToMovement;
	 OriginalGravityScale=GetCharacterMovement()->GravityScale;
	 OriginalWalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
	 OriginalDeceleration= GetCharacterMovement()->BrakingDecelerationFalling ;
	 OriginalAcceleration= GetCharacterMovement()->MaxAcceleration;
	 OriginalAirControl= GetCharacterMovement()->AirControl;
	 OriginalDesiredRotation= GetCharacterMovement()->bUseControllerDesiredRotation;
}

void APlayer1::DescentPlayer()
{
	if (CurrentVelocity.Z != DescendingRate * -1.f && bIsGliding == true)
	{
		CurrentVelocity.Z = UKismetMathLibrary::FInterpTo(CurrentVelocity.Z, DescendingRate, WorldDeltaTime,3.f);
		GetCharacterMovement()->Velocity.Z = DescendingRate * -1.f;
	}
}

void APlayer1::DamageOnFall()
{
	float PlayerVelocity_Z = GetVelocity().Z;
	PlayerVelocity_Z *= -1;
	if (PlayerVelocity_Z > FallHeight) {
		Health = FMath::FloorToInt32(FMath::Clamp(Health - PlayerVelocity_Z / FallDamage, 0.f,MaxHealth));
		UE_LOG
		(
			LogTemp,
			Display,
			TEXT("PlayerHealth is equal :  %f"),Health)
	}
	
}
void APlayer1::ApplyOriginalSetting()
{
	 GetCharacterMovement()->bOrientRotationToMovement = OriginalOrientRotation;
	 GetCharacterMovement()->GravityScale = OriginalGravityScale;
	 GetCharacterMovement()->MaxWalkSpeed=OriginalWalkingSpeed;
	 GetCharacterMovement()->BrakingDecelerationFalling=OriginalDeceleration;
     GetCharacterMovement()->MaxAcceleration=OriginalAcceleration;
	 GetCharacterMovement()->AirControl=OriginalAirControl;
	 GetCharacterMovement()->bUseControllerDesiredRotation=OriginalDesiredRotation;
	 GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
}
void APlayer1::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	//bFirstJump = true;
	DamageOnFall();
	
}

bool APlayer1::IsDead() const
{
	return Health <= 0;
}
