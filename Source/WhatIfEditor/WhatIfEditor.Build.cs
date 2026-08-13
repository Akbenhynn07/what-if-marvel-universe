// Copyright 2026 W.H.A.T. I.F. Project. All Rights Reserved.
// WhatIfEditor Module rules

using UnrealBuildTool;

public class WhatIfEditor : ModuleRules
{
    public WhatIfEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp17;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "WhatIf",
            "UnrealEd",
            "AssetTools",
            "PropertyEditor"
        });
    }
}
