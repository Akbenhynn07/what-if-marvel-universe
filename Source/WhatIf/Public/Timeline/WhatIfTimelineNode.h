#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WhatIfTimelineNode.generated.h"

USTRUCT(BlueprintType)
struct WHATIF_API FWhatIfTimelineNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    FGuid NodeId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    FGuid ParentNodeId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    FText NodeLabel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    int64 SimulationSeed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    TMap<FName, FString> WorldStateSnapshot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    TMap<FName, FString> CharacterStateSnapshots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    TArray<FGuid> ChildNodeIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    FDateTime BranchTimestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    FGameplayTagContainer OutcomeFlags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    FName ScenarioId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    TArray<FString> KeyEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    bool bIsSavePoint = false;

    bool IsRoot() const { return !ParentNodeId.IsValid(); }
};

UCLASS(BlueprintType)
class WHATIF_API UWhatIfTimelineGraph : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    TArray<FWhatIfTimelineNode> Nodes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    FGuid RootNodeId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
    FGuid ActiveNodeId;

    UFUNCTION(BlueprintCallable, Category="Timeline")
    FWhatIfTimelineNode* FindNode(const FGuid& NodeId)
    {
        return Nodes.FindByPredicate([&](const FWhatIfTimelineNode& Node) { return Node.NodeId == NodeId; });
    }

    UFUNCTION(BlueprintCallable, Category="Timeline")
    bool HasNode(const FGuid& NodeId) const
    {
        return Nodes.ContainsByPredicate([&](const FWhatIfTimelineNode& Node) { return Node.NodeId == NodeId; });
    }

    UFUNCTION(BlueprintCallable, Category="Timeline")
    TArray<FWhatIfTimelineNode> GetLeafNodes() const
    {
        TArray<FWhatIfTimelineNode> LeafNodes;
        for (const auto& Node : Nodes)
        {
            if (Node.ChildNodeIds.Num() == 0)
            {
                LeafNodes.Add(Node);
            }
        }
        return LeafNodes;
    }
};
