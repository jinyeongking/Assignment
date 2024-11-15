#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrimaryActionEnums.h"
#include "CDoor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;


UCLASS()
class THIREPERSONCPP_API ACDoor : public AActor
{
    GENERATED_BODY()

public:
    ACDoor();

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

 
    void PerformOpenLerp();

protected:
    // Components
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        USceneComponent* RootSceneComp;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        UStaticMeshComponent* DoorMeshComp; 

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        UBoxComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
        UStaticMeshComponent* FrameMesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
        UStaticMeshComponent* DoorMesh;

    // Properties
    UPROPERTY(EditAnywhere, Category = "Color")
        FLinearColor MaterialColor;

    UPROPERTY(EditAnywhere, Category = "Primary")
        EPrimaryAction PrimaryType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
        EPrimaryAction RequiredKey;

public:
    EPrimaryAction OnPrimaryAction();
    EPrimaryAction GetType() const;
    void FailInteract();
    void SetInteracted();
    bool IsInteracted() const { return bPrimaryAction; }

private:
    UMaterialInstanceDynamic* FrameMaterialInstance;
    UMaterialInstanceDynamic* DoorMaterialInstance;
    bool bPrimaryAction;
    FTimerHandle OpenTimerHandle;
    FRotator CurrentRotation;
    FRotator TargetRotation;
};
