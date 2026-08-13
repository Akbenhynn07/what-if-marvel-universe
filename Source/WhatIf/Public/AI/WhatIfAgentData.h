#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Core/WhatIfTypes.h"
#include "WhatIfAgentData.generated.h"

class UWhatIfCharacterData;

USTRUCT(BlueprintType)
struct WHATIF_API FWhatIfAgentState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent")
    FName CharacterId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent")
    TSoftObjectPtr<UWhatIfCharacterData> CharacterData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent")
    TMap<FGameplayTag, float> CurrentAttributes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent")
    TArray<FName> ActiveModificationIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent")
    TArray<FGameplayTag> ActiveGoals;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    TArray<FName> PerceivedThreatIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    TArray<FName> PerceivedAllyIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    FGameplayTag CurrentThreatLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
    float CurrentEnergyPercent = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
    TMap<FName, float> ResourceLevels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    FGameplayTagContainer StatusFlags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    EWhatIfCombatStance CurrentStance = EWhatIfCombatStance::Balanced;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
    FWhatIfAgentPersonality Personality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationships")
    TArray<FWhatIfRelationshipEntry> CurrentRelationships;
};

USTRUCT(BlueprintType)
struct WHATIF_API FWhatIfAgentAction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    FName ActionId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    FGameplayTag ActionTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    float UtilityScore = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    float EnergyCost = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    FName TargetId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    bool bIsAvailable = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    FText UnavailableReason;
};
