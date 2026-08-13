#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AI/WhatIfAgentData.h"
#include "Core/WhatIfTypes.h"
#include "IDecisionEngine.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UWhatIfDecisionEngine : public UInterface
{
    GENERATED_BODY()
};

class WHATIF_API IWhatIfDecisionEngine
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Decision")
    FWhatIfAgentAction SelectAction(
        const FWhatIfAgentState& AgentState,
        const TArray<FWhatIfAgentAction>& AvailableActions,
        const FWhatIfCombatSetup& CombatContext
    );

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Decision")
    TArray<FWhatIfAgentAction> ScoreAllActions(
        const FWhatIfAgentState& AgentState,
        const TArray<FWhatIfAgentAction>& AvailableActions,
        const FWhatIfCombatSetup& CombatContext
    );

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Decision")
    TArray<FGameplayTag> EvaluateGoals(const FWhatIfAgentState& AgentState);
};
