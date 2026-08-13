#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Core/WhatIfTypes.h"
#include "WhatIfCharacterData.generated.h"

class UTexture2D;
class USkeletalMesh;
class UAnimInstance;
class UMaterialInterface;
class UWhatIfAbilityData;
class UWhatIfEquipmentData;

UCLASS(BlueprintType)
class WHATIF_API UWhatIfCharacterData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // Identity section
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FName CharacterId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FText Lore;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    TSoftObjectPtr<UTexture2D> Portrait;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    EWhatIfFaction DefaultFaction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FGameplayTagContainer CharacterTags;

    // Visual section
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
    TSoftObjectPtr<USkeletalMesh> BaseMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
    TSoftClassPtr<UAnimInstance> BaseAnimInstanceClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
    TSoftObjectPtr<UMaterialInterface> BaseMaterial;

    // Base Attributes section
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
    TMap<FGameplayTag, float> BaseAttributeValues;

    // Loadout section
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loadout")
    TArray<TSoftObjectPtr<UWhatIfAbilityData>> DefaultAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loadout")
    TArray<TSoftObjectPtr<UWhatIfEquipmentData>> DefaultEquipment;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loadout")
    EWhatIfMobilityClass DefaultMobilityClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loadout")
    EWhatIfEnergyType PrimaryEnergyType;

    // AI section
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    FWhatIfAgentPersonality Personality;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    EWhatIfCombatStance DefaultCombatStance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    float BaseReactionTime;

    // Relationships section
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Relationships")
    TArray<FWhatIfRelationshipEntry> BaseRelationships;

    // Constraints section
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constraints")
    FGameplayTagContainer IncompatibleModificationTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constraints")
    float MaxCarryWeight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constraints")
    float BaseEnergyCapacity;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(FPrimaryAssetType("WhatIfCharacterData"), GetFName());
    }
};
