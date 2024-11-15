#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Backstepped.generated.h"

UCLASS()
class THIREPERSONCPP_API UCAnimNotify_Backstepped : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
