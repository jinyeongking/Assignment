#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CAttributeComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "ACtions/CEquipment.h"
#include "Assignment/CChest.h"
#include "Assignment/CDoor.h"


ACPlayer::ACPlayer()
{
	// Spring ArmComp
	CHelpers::CreateSceneComponent(this, &SpringArmComp, "SpringArmComp", GetMesh());
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 140));
	SpringArmComp->AddRelativeRotation(FRotator(0, 90, 0));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;

	// Camera Comp
	CHelpers::CreateSceneComponent(this, &CameraComp, "CameraComp", SpringArmComp);

	//MeshComp
	USkeletalMesh* MeshAsset;
	CHelpers::GetAsset(&MeshAsset, "/Game/Character/Mesh/SK_Mannequin");
	GetMesh()->SetSkeletalMesh(MeshAsset);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	// Animation
	TSubclassOf<UAnimInstance> AnimClass;
	CHelpers::GetClass(&AnimClass, "/Game/Player/ABP_CPlayer");
	GetMesh()->SetAnimInstanceClass(AnimClass);

	// Action Comp
	CHelpers::CreateActorComponent(this, &ActionComp, "ActionComp");

	// Montage Comp
	CHelpers::CreateActorComponent(this, &MontagesComp, "MontagesComp");

	// Attribute Comp
	CHelpers::CreateActorComponent(this, &AttributeComp, "AttributeComp");

	// Option Comp
	CHelpers::CreateActorComponent(this, &OptionComp, "OptionComp");

	// State Comp
	CHelpers::CreateActorComponent(this, &StateComp, "StateComp");

	// Movement Comp
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// Widget Class

	CollectedKeys.Add(EPrimaryAction::Red, false);
	CollectedKeys.Add(EPrimaryAction::Green, false);
	CollectedKeys.Add(EPrimaryAction::Blue, false);
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Set Dynamic Material
	BodyMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), nullptr);
	LogoMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), nullptr);
	
	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	// On StateType Changed
	StateComp->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	ActionComp->SetUnarmedMode();

	// 상호 작용 항상 활성화
	Keys.SetNum((int32)EPrimaryAction::MAX);
	Keys[(int32)EPrimaryAction::None] = true;

	
}

void ACPlayer::SetBodyColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}


void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACPlayer::OnTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACPlayer::OnLookUp);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	PlayerInputComponent->BindAction("Evade", IE_Pressed, this, &ACPlayer::OnEvade);
	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &ACPlayer::OnWalk);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &ACPlayer::OffWalk);

	PlayerInputComponent->BindAction("Fist", IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("OneHand", IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("TwoHand", IE_Pressed, this, &ACPlayer::OnTwoHand);
	PlayerInputComponent->BindAction("MagicBall", IE_Pressed, this, &ACPlayer::OnMagicBall);
	PlayerInputComponent->BindAction("Warp", IE_Pressed, this, &ACPlayer::OnWarp);
	PlayerInputComponent->BindAction("WhirlWind", IE_Pressed, this, &ACPlayer::OnWhirlWind);

	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ACPlayer::OnPrimaryAction);
}



void ACPlayer::OnMoveForward(float Axis)
{
	CheckFalse(AttributeComp->IsCanMove());

	FRotator ControlRot = FRotator(0, GetControlRotation().Yaw, 0);
	FVector Direction = FQuat(ControlRot).GetForwardVector();

	AddMovementInput(Direction, Axis);
}

void ACPlayer::OnMoveRight(float Axis)
{
	CheckFalse(AttributeComp->IsCanMove());

	FRotator ControlRot = FRotator(0, GetControlRotation().Yaw, 0);
	FVector Direction = FQuat(ControlRot).GetRightVector();

	AddMovementInput(Direction, Axis);
}

void ACPlayer::OnTurn(float Axis)
{
	float rate = Axis * OptionComp->GetMouseXSpeed() * GetWorld()->GetDeltaSeconds();

	AddControllerYawInput(rate);
}

void ACPlayer::OnLookUp(float Axis)
{
	float rate = Axis * OptionComp->GetMouseYSpeed() * GetWorld()->GetDeltaSeconds();

	AddControllerPitchInput(rate);
}

void ACPlayer::OnZoom(float Axis)
{
	float Rate = OptionComp->GetZoomSpeed() * Axis * GetWorld()->GetDeltaSeconds();

	SpringArmComp->TargetArmLength += Rate;
	
	SpringArmComp->TargetArmLength = FMath::Clamp(SpringArmComp->TargetArmLength, OptionComp->GetZoomMin(), OptionComp->GetZoomMax());
}

void ACPlayer::OnEvade()
{
	CheckFalse(StateComp->IsIdleMode());
	CheckFalse(AttributeComp->IsCanMove());

	if (InputComponent->GetAxisValue("MoveForward") < 0)
	{
		StateComp->SetBackstepMode();
			return;
	}


	StateComp->SetRollMode();
}

void ACPlayer::OnWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetWalkSpeed();
}

void ACPlayer::OffWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetSprintSpeed();
}

void ACPlayer::OnFist()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetFistMode();
}

void ACPlayer::OnOneHand()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetOneHandMode();
}

void ACPlayer::OnTwoHand()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetTwoHandMode();
}

void ACPlayer::OnMagicBall()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetMagicBallMode();
}

void ACPlayer::OnWarp()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetWarpMode();
}

void ACPlayer::OnWhirlWind()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetWhirlWindMode();
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	RollingRotation();

	MontagesComp->PlayRoll();
}

void ACPlayer::Begin_Backstep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	MontagesComp->PlayBackstep();
}

void ACPlayer::RollingRotation()
{
	FVector Start, Target;
	FRotator Rotation;

	Start = GetActorLocation();
	if (GetVelocity().IsNearlyZero())
	{
		const FRotator& ControlRotation=FRotator(0, GetControlRotation().Yaw, 0);
		const FVector& ControlForward = FQuat(ControlRotation).GetForwardVector();

		Target = Start + ControlForward;
	}
	else
	{
		Target = Start + GetVelocity().GetSafeNormal2D();
	}
	Rotation=UKismetMathLibrary::FindLookAtRotation(Start, Target);

	SetActorRotation(Rotation);
}

void ACPlayer::End_Roll()
{
	UCActionData* CurrentDA = ActionComp->GetCurrentDataAsset();

	if (CurrentDA&&CurrentDA->EquipmentData.bUseControlRotation)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	StateComp->SetIdleMode();
}

void ACPlayer::End_Backstep()
{
	UCActionData* CurrentDA = ActionComp->GetCurrentDataAsset();

	if (CurrentDA && !CurrentDA->EquipmentData.bUseControlRotation)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	StateComp->SetIdleMode();
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Roll:
		{
			Begin_Roll();
		}
		break;

		case EStateType::Backstep:
		{
			Begin_Backstep();
		}
		break;
	}
}

void ACPlayer::CollectKey(EPrimaryAction KeyType) {
	if (KeyType != EPrimaryAction::None && !CollectedKeys[KeyType])
	{
		CollectedKeys[KeyType] = true;
		UE_LOG(LogTemp, Warning, TEXT("Player collected a key of type: %s"), *UEnum::GetValueAsString(KeyType));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to collect key or key already collected!"));
	}
}

bool ACPlayer::HasKey(EPrimaryAction KeyType) const {
	const bool* HasKeyPtr = CollectedKeys.Find(KeyType);
	return HasKeyPtr && *HasKeyPtr;
}

void ACPlayer::OnPrimaryAction() {
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * InteractionRange;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		AActor* HitActor = HitResult.GetActor();
		if (ACChest* Chest = Cast<ACChest>(HitActor))
		{
			EPrimaryAction CollectedKey = Chest->OnPrimaryAction();
			if (CollectedKey != EPrimaryAction::None)
			{
				CollectKey(CollectedKey);
			}
		}
		else if (ACDoor* Door = Cast<ACDoor>(HitActor))
		{
			EPrimaryAction Result = Door->OnPrimaryAction();
			if (Result == EPrimaryAction::None)
			{
				Door->FailInteract();  //플레이어에게 올바른 키가 없다고 알려줍니다.
			}
		}
	}
}