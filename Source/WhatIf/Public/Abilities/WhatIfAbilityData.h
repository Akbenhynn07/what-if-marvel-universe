#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Core/WhatIfTypes.h"
#include "WhatIfAbilityData.generated.h"

class UTexture2D;
class UGameplayAbility;

UCLASS(BlueprintType)
class WHATIF_API UWhatIfAbilityData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FName AbilityId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TSoftClassPtr<UGameplayAbility> AbilityClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FGameplayTagContainer AbilityTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FGameplayTagContainer RequiredCharacterTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float EnergyCost;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float Cooldown;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float Range;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
    TArray<FText> Advantages;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
    TArray<FText> Disadvantages;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    EWhatIfEnergyType EnergyType;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(FPrimaryAssetType("WhatIfAbilityData"), GetFName());
    }
};
