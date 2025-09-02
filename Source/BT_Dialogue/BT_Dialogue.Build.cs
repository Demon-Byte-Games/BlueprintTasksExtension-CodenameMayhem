using UnrealBuildTool;
using System.IO;       // Required for Directory and Path

public class BT_Dialogue : ModuleRules
{
    public BT_Dialogue(ReadOnlyTargetRules Target) : base(Target)
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
        
        string PluginPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../"));
        
        //Check if the Hermes plugin exists
        if (Directory.Exists(Path.Combine(PluginPath, "TagFacts")))
        {
            PublicDefinitions.Add("TAGFACTS_INSTALLED=1");
            PublicDependencyModuleNames.Add("TagFacts");
        }
        else
        {
            PublicDefinitions.Add("TAGFACTS_INSTALLED=0");
        }
    }
}