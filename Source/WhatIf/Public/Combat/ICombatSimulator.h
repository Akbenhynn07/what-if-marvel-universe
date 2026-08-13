#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/WhatIfTypes.h"
#include "ICombatSimulator.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UWhatIfCombatSimulator : public UInterface
{
    GENERATED_BODY()
};

class WHATIF_API IWhatIfCombatSimulator
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Combat")
    FWhatIfCombatResult RunCombat(const FWhatIfCombatSetup& Setup, int64 RandomSeed);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Combat")
    FWhatIfCombatResult RunSingleRound(const FWhatIfCombatSetup& Setup, const FWhatIfCombatResult& CurrentState, int64 RandomSeed);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Combat")
    bool ValidateCombatSetup(const FWhatIfCombatSetup& Setup, FText& OutError) const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Combat")
    int32 GetMaxRounds() const;
};
