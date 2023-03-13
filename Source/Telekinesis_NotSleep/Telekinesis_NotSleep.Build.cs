// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Telekinesis_NotSleep : ModuleRules
{
	public Telekinesis_NotSleep(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
