// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DemoVer1_Huawei : ModuleRules
{
	public DemoVer1_Huawei(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
    }
}
