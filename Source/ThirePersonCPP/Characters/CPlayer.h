#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Interfaces/CCharacterInterface.h"
#include "Interfaces/CInteractableInterface.h"
#include "Assignment/PrimaryActionEnums.h"
#include "CPlayer.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UCAttributeComponent;
class UCOptionComponent;
class UCStateComponent;
class UCMontagesComponent;
class UCActionComponent;

UCLASS()
class THIREPERSONCPP_API ACPlayer : public ACharacter, public ICCharacterInterface
{
    GENERATED_BODY()

public:
    ACPlayer();

    virtual void SetBodyColor(FLinearColor InColor) override;

protected:
    virtual void BeginPlay() override;

public:

    // 플레이어에 특정 키가 있는지 확인
    bool HasKey(EPrimaryAction KeyType) const;
    // 플레이어에게 키를 부여하는 기능
    void CollectKey(EPrimaryAction KeyType);




    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
        float InteractionRange = 200.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keys")
        TMap<EPrimaryAction, bool> CollectedKeys;
public:
    void OnPrimaryAction();

private:
    void OnMoveForward(float Axis);
    void OnMoveRight(float Axis);
    void OnTurn(float Axis);
    void OnLookUp(float Axis);
    void OnZoom(float Axis);

    void OnEvade();
    void OnWalk();
    void OffWalk();

    void OnFist();
    void OnOneHand();
    void OnTwoHand();
    void OnMagicBall();
    void OnWarp();
    void OnWhirlWind();
    void OnInteracting();

private:
    void Begin_Roll();
    void Begin_Backstep();
    void RollingRotation();

public:
    void End_Roll();
    void End_Backstep();

private:
    UFUNCTION()
        void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        UCameraComponent* CameraComp;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        UCAttributeComponent* AttributeComp;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        UCOptionComponent* OptionComp;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        UCStateComponent* StateComp;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        UCMontagesComponent* MontagesComp;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        UCActionComponent* ActionComp;

   
    UPROPERTY(VisibleInstanceOnly, Category = "Key")
        TArray<bool> Keys;


  

private:
    UMaterialInstanceDynamic* BodyMaterial;
    UMaterialInstanceDynamic* LogoMaterial;
};