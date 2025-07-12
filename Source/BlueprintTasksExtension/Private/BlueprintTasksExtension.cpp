// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintTasksExtension.h"

#include "BNTEditorDeveloperSettings.h"

#define LOCTEXT_NAMESPACE "FBlueprintTasksExtensionModule"

void FBlueprintTasksExtensionModule::StartupModule()
{
	#if WITH_EDITOR
	FCoreDelegates::OnPostEngineInit.AddLambda([this]()
	{
		UBNTEditorDeveloperSettings* DeveloperSettings = GetMutableDefault<UBNTEditorDeveloperSettings>();
		DeveloperSettings->ExtraTaskPaletteFunctions.Add("Async Start Task Graph");
	});
	#endif
	
}

void FBlueprintTasksExtensionModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlueprintTasksExtensionModule, BlueprintTasksExtension)