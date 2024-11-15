#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CActionData.generated.h"

class UAnimMontage;
class ACharacter;
class ACEquipment;
class ACAttachment;

USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	FName StartSection;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bUseControlRotation = true;
};

UCLASS()
class THIREPERSONCPP_API UCActionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	void BeginPlay(ACharacter* InOwnerCharacter);

public:
	FORCEINLINE ACEquipment* GetEquipment() { return Equipment; }
private:
	FString MakeLabel(ACharacter* InOwnerCharacter,FString InMiddleName);

public:
	// Equipment
	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<ACEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FLinearColor EquipmentColor;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FEquipmentData EquipmentData;

	// Attachment
	UPROPERTY(EditAnywhere, Category = "Attachment")
	TSubclassOf<ACAttachment> AttachmentClass;

private:
	ACEquipment* Equipment;
	ACAttachment* Attachment;
};
