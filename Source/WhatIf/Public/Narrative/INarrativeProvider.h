#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/WhatIfTypes.h"
#include "Timeline/WhatIfTimelineNode.h"
#include "INarrativeProvider.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnNarrativeReady, bool, bSuccess, const FString&, NarrativeText);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnDialogueReady, bool, bSuccess, const FString&, DialogueLine);

UINTERFACE(MinimalAPI, Blueprintable)
class UWhatIfNarrativeProvider : public UInterface
{
    GENERATED_BODY()
};

class WHATIF_API IWhatIfNarrativeProvider
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="WhatIf|Narrative")
    void GenerateNarrative(
        const FWhatIfSimulationResult& Result,
        const FOnNarrativeReady& OnComplete
    );

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="WhatIf|Narrative")
    void GenerateDialogue(
        const FName& CharacterId,
        const FWhatIfAgentPersonality& Personality,
        const FString& SituationContext,
        const FOnDialogueReady& OnComplete
    );

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="WhatIf|Narrative")
    void GenerateTimelineSummary(
        const FWhatIfTimelineNode& Node,
        const FOnNarrativeReady& OnComplete
    );

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="WhatIf|Narrative")
    bool IsProviderAvailable() const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="WhatIf|Narrative")
    FString GetProviderName() const;
};
