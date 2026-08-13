#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/WhatIfTypes.h"
#include "ITransformationSystem.generated.h"

class UWhatIfModificationData;

UINTERFACE(MinimalAPI, Blueprintable)
class UWhatIfTransformationSystem : public UInterface
{
    GENERATED_BODY()
};

class WHATIF_API IWhatIfTransformationSystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Transformation")
    bool ApplyModification(const UWhatIfModificationData* ModData, const FName& CharacterId);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Transformation")
    bool RemoveModification(const FName& ModificationId, const FName& CharacterId);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Transformation")
    bool IsModificationCompatible(const UWhatIfModificationData* ModData, const FName& CharacterId, FText& OutReason) const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Transformation")
    TArray<FName> GetActiveModifications(const FName& CharacterId) const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Transformation")
    TArray<FWhatIfAttributeModifier> PreviewModificationEffect(const UWhatIfModificationData* ModData, const FName& CharacterId) const;
};
