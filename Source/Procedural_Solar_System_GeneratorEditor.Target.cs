// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Procedural_Solar_System_GeneratorEditorTarget  : TargetRules
{
	public Procedural_Solar_System_GeneratorEditorTarget ( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("Procedural_Solar_System_Generator");
	}
}
