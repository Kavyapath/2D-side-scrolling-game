// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Tesst_AlienDog : ModuleRules
{
	public Tesst_AlienDog(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput","AIModule","CableComponent" });
	}
}
