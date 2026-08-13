#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WhatIfAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class WHATIF_API UWhatIfAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UWhatIfAttributeSet();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Attributes")
    FGameplayAttributeData Strength;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, Strength)
    UFUNCTION() void OnRep_Strength(const FGameplayAttributeData& OldStrength) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, Strength, OldStrength); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Speed, Category = "Attributes")
    FGameplayAttributeData Speed;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, Speed)
    UFUNCTION() void OnRep_Speed(const FGameplayAttributeData& OldSpeed) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, Speed, OldSpeed); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Durability, Category = "Attributes")
    FGameplayAttributeData Durability;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, Durability)
    UFUNCTION() void OnRep_Durability(const FGameplayAttributeData& OldDurability) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, Durability, OldDurability); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Energy, Category = "Attributes")
    FGameplayAttributeData Energy;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, Energy)
    UFUNCTION() void OnRep_Energy(const FGameplayAttributeData& OldEnergy) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, Energy, OldEnergy); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy, Category = "Attributes")
    FGameplayAttributeData MaxEnergy;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, MaxEnergy)
    UFUNCTION() void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, MaxEnergy, OldMaxEnergy); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Attributes")
    FGameplayAttributeData Intelligence;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, Intelligence)
    UFUNCTION() void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, Intelligence, OldIntelligence); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DefenseRating, Category = "Attributes")
    FGameplayAttributeData DefenseRating;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, DefenseRating)
    UFUNCTION() void OnRep_DefenseRating(const FGameplayAttributeData& OldDefenseRating) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, DefenseRating, OldDefenseRating); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackRating, Category = "Attributes")
    FGameplayAttributeData AttackRating;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, AttackRating)
    UFUNCTION() void OnRep_AttackRating(const FGameplayAttributeData& OldAttackRating) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, AttackRating, OldAttackRating); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MobilityRating, Category = "Attributes")
    FGameplayAttributeData MobilityRating;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, MobilityRating)
    UFUNCTION() void OnRep_MobilityRating(const FGameplayAttributeData& OldMobilityRating) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, MobilityRating, OldMobilityRating); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ReactionTime, Category = "Attributes")
    FGameplayAttributeData ReactionTime;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, ReactionTime)
    UFUNCTION() void OnRep_ReactionTime(const FGameplayAttributeData& OldReactionTime) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, ReactionTime, OldReactionTime); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Accuracy, Category = "Attributes")
    FGameplayAttributeData Accuracy;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, Accuracy)
    UFUNCTION() void OnRep_Accuracy(const FGameplayAttributeData& OldAccuracy) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, Accuracy, OldAccuracy); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RecoveryRate, Category = "Attributes")
    FGameplayAttributeData RecoveryRate;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, RecoveryRate)
    UFUNCTION() void OnRep_RecoveryRate(const FGameplayAttributeData& OldRecoveryRate) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, RecoveryRate, OldRecoveryRate); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TacticalCapability, Category = "Attributes")
    FGameplayAttributeData TacticalCapability;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, TacticalCapability)
    UFUNCTION() void OnRep_TacticalCapability(const FGameplayAttributeData& OldTacticalCapability) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, TacticalCapability, OldTacticalCapability); }

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResourceEfficiency, Category = "Attributes")
    FGameplayAttributeData ResourceEfficiency;
    ATTRIBUTE_ACCESSORS(UWhatIfAttributeSet, ResourceEfficiency)
    UFUNCTION() void OnRep_ResourceEfficiency(const FGameplayAttributeData& OldResourceEfficiency) { GAMEPLAYATTRIBUTE_REPNOTIFY(UWhatIfAttributeSet, ResourceEfficiency, OldResourceEfficiency); }
};
