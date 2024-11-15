#pragma once

#include "CoreMinimal.h"
#include "PrimaryActionEnums.generated.h" 

UENUM(BlueprintType)
enum class EPrimaryAction : uint8
{
    None,
    Red,
    Green,
    Blue,
    MAX
};