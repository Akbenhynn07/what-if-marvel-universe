// Copyright 2026 W.H.A.T. I.F. Project. All Rights Reserved.
// WhatIf Module rules

using UnrealBuildTool;

public class WhatIf : ModuleRules
{
	public WhatIf(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks", 
			"ModularGameplay", 
			"GameFeatures", 
			"StructUtils"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"UMG", 
			"Slate", 
			"SlateCore"
		});
	}
}
