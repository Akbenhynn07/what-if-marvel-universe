#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WhatIfTypes.generated.h"

UENUM(BlueprintType)
enum class EWhatIfFaction : uint8
{
    Unaffiliated,
    Hero,
    Villain,
    Neutral,
    Cosmic,
    Synthetic
};

UENUM(BlueprintType)
enum class EWhatIfMobilityClass : uint8
{
    Grounded,
    Flight,
    Teleportation,
    SpeedRunning,
    Aquatic,
    Space
};

UENUM(BlueprintType)
enum class EWhatIfEnergyType : uint8
{
    None,
    Solar,
    Arc,
    Mystic,
    Cosmic,
    Neural,
    Biological,
    Mechanical,
    Quantum
};

UENUM(BlueprintType)
enum class EWhatIfModificationSlot : uint8
{
    ArmorLayer,
    WeaponSystem,
    PowerSource,
    DefenseSystem,
    MobilitySystem,
    AISystem,
    SpecialAbility,
    SupportSystem
};

UENUM(BlueprintType)
enum class EWhatIfCombatStance : uint8
{
    Aggressive,
    Defensive,
    Balanced,
    Protective,
    Evasive,
    Supportive
};

UENUM(BlueprintType)
enum class EWhatIfRelationshipType : uint8
{
    Ally,
    Enemy,
    Rival,
    Neutral,
    Unknown
};

UENUM(BlueprintType)
enum class EWhatIfModifierOp : uint8
{
    Add,
    Multiply,
    Override
};

USTRUCT(BlueprintType)
struct WHATIF_API FWhatIfStatRange
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Min = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Max = 0.0f;
};

USTRUCT(BlueprintType)
struct WHATIF_API FWhatIfAttributeModifier
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
    FGameplayTag AttributeTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
    float BaseValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
    float ModifierValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
    EWhatIfModifierOp Op = EWhatIfModifierOp::Add;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
    FText Source;
};

USTRUCT(BlueprintType)
struct WHATIF_API FWhatIfAgentPersonality
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Aggression = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Loyalty = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Recklessness = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Creativity = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SelfPreservation = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Altruism = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
    FText PersonalityDescription;
};

USTRUCT(BlueprintType)
struct WHATIF_API FWhatIfRelationshipEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
    FName TargetCharacterId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
    float AffinityValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
    EWhatIfRelationshipType Type = EWhatIfRelationshipType::Neutral;
};

USTRUCT(BlueprintType)
struct WHATIF_API FWhatIfSimulationResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
    FGuid TimelineNodeId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
    int64 Seed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
    TArray<FName> ParticipantIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
    TMap<FName, float> FinalAttributeValues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
    FGameplayTagContainer OutcomeFlags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
    TArray<FString> KeyEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
    float SimulationDurationSeconds = 0.0f;
};

USTRUCT(BlueprintType)
struct WHATIF_API FWhatIfCombatSetup
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<FName> AttackerIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<FName> DefenderIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FName EnvironmentId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FName ScenarioId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int64 Seed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TMap<FName, FGameplayTagContainer> ActiveModifications;
};

USTRUCT(BlueprintType)
struct WHATIF_API FWhatIfCombatResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bCombatResolved = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<FName> WinnerIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<FName> LoserIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<FName> SurvivorIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<FString> CombatLog;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TMap<FName, float> AttributeChanges;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FGameplayTagContainer OutcomeFlags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 RoundsElapsed = 0;
};
