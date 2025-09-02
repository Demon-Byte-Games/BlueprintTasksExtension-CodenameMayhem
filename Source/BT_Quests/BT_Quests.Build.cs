﻿using UnrealBuildTool;
using System.IO;       // Required for Directory and Path

public class BT_Quests : ModuleRules
{
    public BT_Quests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "BlueprintTaskForge",
                "GameplayTags"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
        
        //Path to the current plugin directory
        string PluginPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../"));
        
        //Check if the TagFacts plugin exists
        if (Directory.Exists(Path.Combine(PluginPath, "TagFacts")))
        {
            PublicDefinitions.Add("TAGFACTS_INSTALLED=1");
            PublicDependencyModuleNames.Add("TagFacts");
        }
        else
        {
            PublicDefinitions.Add("TAGFACTS_INSTALLED=0");
        }
		
        //Check if the Hermes plugin exists
        if (Directory.Exists(Path.Combine(PluginPath, "HermesCommunications")))
        {
            PublicDefinitions.Add("HERMES_INSTALLED=1");
            PublicDependencyModuleNames.Add("HermesCommunications");
        }
        else
        {
            PublicDefinitions.Add("HERMES_INSTALLED=0");
        }
        
        //Check if the Cog plugin exists
        if (Directory.Exists(Path.Combine(PluginPath, "Cog")))
        {
            PublicDefinitions.Add("COG_INSTALLED=1");
            PublicDependencyModuleNames.Add("CogCommon");
            // Other Cog plugins can be added only for specific target configuration
            if (Target.Configuration != UnrealTargetConfiguration.Shipping)
            {
                PublicDependencyModuleNames.AddRange(new string[]
                {
                    "Cog",
                    "CogDebug",
                    "CogEngine",
                    "CogImgui",
                });
            }
        }
        else
        {
            PublicDefinitions.Add("COG_INSTALLED=0");
        }
    }
}