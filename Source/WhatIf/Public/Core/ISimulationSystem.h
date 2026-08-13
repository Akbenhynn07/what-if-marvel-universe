#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISimulationSystem.generated.h"

// U-class stub
UINTERFACE(MinimalAPI, Blueprintable)
class UWhatIfSimulationSystem : public UInterface 
{ 
    GENERATED_BODY() 
};

// I-class logic
class WHATIF_API IWhatIfSimulationSystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Simulation")
    void InitializeSystem();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Simulation")
    void ShutdownSystem();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Simulation")
    bool IsSystemReady() const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WhatIf|Simulation")
    FString GetSystemName() const;
};
