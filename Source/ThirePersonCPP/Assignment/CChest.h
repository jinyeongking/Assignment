#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrimaryActionEnums.h"
#include "CChest.generated.h"

class UBoxComponent;
class UStaticMeshComponent;



UCLASS()
class THIREPERSONCPP_API ACChest : public AActor
{
    GENERATED_BODY()

public:
    ACChest();

protected:
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;



private:
    UFUNCTION()
        void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
        void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
    UFUNCTION(BlueprintNativeEvent)
        void Open();

protected:
    // Components
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        USceneComponent* RootSceneComp;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        UBoxComponent* BoxComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
        UStaticMeshComponent* UpMeshComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
        UStaticMeshComponent* DownMeshComp;

    
    UPROPERTY(EditAnywhere, Category = "Color")
        FLinearColor Emissive;

    UPROPERTY(EditAnywhere, Category = "Interact")
        EPrimaryAction Type;

public:
    // 상호 작용 기능(이전 인터페이스)
    EPrimaryAction OnPrimaryAction();
    EPrimaryAction GetType() const;
    void SetInteracted();
    bool IsInteracted() const { return bPrimaryAction; }
    void FailInteract();


private:
  
    UMaterialInstanceDynamic* DownMaterial;
    UMaterialInstanceDynamic* UpMaterial;

   
    bool bPrimaryAction;
};