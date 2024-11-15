#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

class CPlayer;

UCLASS()
class THIREPERSONCPP_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAttachment();

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION(BlueprintCallable)
	void ActorAttachTo(FName InSocketName);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnEquip();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUnequip();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadOnly)
	ACharacter* OwnerCharacter;
};
