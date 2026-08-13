#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Core/WhatIfTypes.h"
#include "WhatIfModificationData.generated.h"

class UTexture2D;
class UGameplayEffect;

UCLASS(BlueprintType)
class WHATIF_API UWhatIfModificationData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modification")
    FName ModificationId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modification")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modification")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modification")
    FText TechnicalSpecification;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modification")
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modification")
    EWhatIfModificationSlot Slot;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
    FGameplayTagContainer ModificationTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
    FGameplayTagContainer RequiredCharacterTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
    FGameplayTagContainer IncompatibleTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    TSoftClassPtr<UGameplayEffect> AttributeEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    TArray<FWhatIfAttributeModifier> AttributeModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
    TArray<FText> Advantages;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
    TArray<FText> Disadvantages;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
    TArray<FText> SideEffects;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float EnergyCost;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float EnergyDrainPerSecond;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float Weight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float ActivationCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
    bool bIsExperimental;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Requirements")
    FWhatIfStatRange CompatibleEnergyCapacityRange;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(FPrimaryAssetType("WhatIfModificationData"), GetFName());
    }
};
