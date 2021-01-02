// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FovTutorial : ModuleRules
{
	public FovTutorial(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
