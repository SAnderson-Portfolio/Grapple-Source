// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Inventory : ModuleRules
{
	public Inventory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "CableComponent" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "CableComponent" });
    }
}
