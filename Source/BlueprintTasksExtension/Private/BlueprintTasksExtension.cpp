﻿// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintTasksExtension.h"

#include "Settings/BtfEditorSettings.h"

#define LOCTEXT_NAMESPACE "FBlueprintTasksExtensionModule"

void FBlueprintTasksExtensionModule::StartupModule()
{
	#if WITH_EDITOR
	FCoreDelegates::OnPostEngineInit.AddLambda([this]()
	{
		UBtf_EditorSettings* DeveloperSettings = GetMutableDefault<UBtf_EditorSettings>();
		DeveloperSettings->ExtraTaskPaletteFunctions.Add("Async Start Task Graph");
	});
	#endif
	
}

void FBlueprintTasksExtensionModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlueprintTasksExtensionModule, BlueprintTasksExtension)