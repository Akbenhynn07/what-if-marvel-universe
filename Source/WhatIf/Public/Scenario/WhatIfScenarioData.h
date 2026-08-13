#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Core/WhatIfTypes.h"
#include "WhatIfScenarioData.generated.h"

class UWhatIfEnvironmentData;

USTRUCT(BlueprintType)
struct WHATIF_API FWhatIfScenarioEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scenario")
    FName EventId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scenario")
    FText EventName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scenario")
    FText EventDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scenario")
    FGameplayTagContainer TriggerConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scenario")
    FGameplayTagContainer OutcomeFlags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scenario")
    float TriggerProbability = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scenario")
    TArray<FName> AffectedCharacterIds;
};

UCLASS(BlueprintType)
class WHATIF_API UWhatIfScenarioData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Scenario")
    FName ScenarioId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Scenario")
    FText ScenarioName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Scenario")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Participants")
    TArray<FName> RequiredCharacterIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Participants")
    TArray<FName> OptionalCharacterIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Participants")
    TMap<FName, EWhatIfFaction> CharacterFactionOverrides;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="World")
    TSoftObjectPtr<UWhatIfEnvironmentData> Environment;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objectives")
    TArray<FGameplayTag> PrimaryObjectives;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objectives")
    TArray<FGameplayTag> SecondaryObjectives;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rules")
    FGameplayTagContainer ScenarioConstraints;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Events")
    TArray<FWhatIfScenarioEvent> PossibleEvents;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
    TMap<FGameplayTag, float> InitialWorldState;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Outcomes")
    TArray<FGameplayTag> PossibleOutcomes;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(FPrimaryAssetType("WhatIfScenarioData"), GetFName());
    }
};
