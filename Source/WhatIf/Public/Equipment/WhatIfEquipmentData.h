#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Core/WhatIfTypes.h"
#include "WhatIfEquipmentData.generated.h"

class UTexture2D;
class UStaticMesh;
class UGameplayEffect;

UCLASS(BlueprintType)
class WHATIF_API UWhatIfEquipmentData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    FName EquipmentId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    EWhatIfModificationSlot Slot;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    FGameplayTagContainer EquipmentTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    TSoftClassPtr<UGameplayEffect> PassiveEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float Weight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float EnergyCostPerSecond;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constraints")
    FGameplayTagContainer IncompatibleEquipmentTags;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(FPrimaryAssetType("WhatIfEquipmentData"), GetFName());
    }
};
