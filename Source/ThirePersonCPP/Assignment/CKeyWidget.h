#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CKeyWidget.generated.h"

class UImage;

UCLASS()
class THIREPERSONCPP_API UCKeyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ActiveKeyIcon(int32 index);
};
