#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "WhatIfEnvironmentData.generated.h"

class UWorld;

UCLASS(BlueprintType)
class WHATIF_API UWhatIfEnvironmentData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    FName EnvironmentId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    TSoftObjectPtr<UWorld> Level;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    FGameplayTagContainer EnvironmentTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    TMap<FGameplayTag, float> AttributeModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    TArray<FGameplayTag> DisabledAbilityTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    float GravityScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    bool bAllowFlight = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    bool bIsSpace = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    float VisibilityRange = 10000.0f;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(FPrimaryAssetType("WhatIfEnvironmentData"), GetFName());
    }
};
