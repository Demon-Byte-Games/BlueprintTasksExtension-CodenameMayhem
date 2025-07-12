// Copyright (C) Varian Daemon 2025. All Rights Reserved.


#include "QuestCheatExtension.h"

#include "BT_Quests.h"
#include "QuestSystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/AssetManager.h"

UQuestCheatExtension::UQuestCheatExtension()
{
#if UE_WITH_CHEAT_MANAGER
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		UCheatManager::RegisterForOnCheatManagerCreated(FOnCheatManagerCreated::FDelegate::CreateLambda(
		[](UCheatManager* CheatManager)
		{
			CheatManager->AddCheatManagerExtension(NewObject<ThisClass>(CheatManager));
		}));
	}
#endif
}

void UQuestCheatExtension::SetQuestState(const FString& PartialQuestName, const FString& NewState)
{
	if(NewState != "Inactive"
		|| NewState != "InProgress"
		|| NewState != "Completed"
		|| NewState != "Failed"
		|| NewState != "Locked")
	{
		return;
	}

	TArray<FAssetData> AssetDataList;
	FARFilter Filter;
	Filter.ClassPaths.Add(UQuestAsset::StaticClass()->GetClassPathName());
	
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	AssetRegistry.GetAssets(Filter, AssetDataList);

	FAssetData MatchingAssetData;
	for(const FAssetData& AssetData : AssetDataList)
	{
		//Check if asset name contains the partial string (case-sensitive or insensitive as needed)
		if (AssetData.AssetName.ToString().Contains(PartialQuestName))
		{
			//Found the asset
			MatchingAssetData = AssetData;
			break;
		}
	}

	if(MatchingAssetData.IsValid())
	{
		//Convert to soft object path
		FSoftObjectPath AssetPath = MatchingAssetData.ToSoftObjectPath();

		// If this asset is a primary asset (i.e. its class overrides GetPrimaryAssetId),
		// you can now load it via the Asset Manager:
		UAssetManager& Manager = UAssetManager::Get();

		// Here you could create a delegate for async loading; for example:
		TArray<FName> Bundles;
		Manager.LoadPrimaryAsset(MatchingAssetData.GetPrimaryAssetId(), Bundles,
			FStreamableDelegate::CreateLambda([AssetPath, NewState]()
			{
				// Once loaded, get the asset object
				// UObject* LoadedAsset = AssetPath.ResolveObject();
				UQuestAsset* QuestAsset = Cast<UQuestAsset>(AssetPath.ResolveObject());

				UQuestSystem* QuestSubSystem = UQuestSystem::Get();
				if(!QuestSubSystem)
				{
					return;
				}

				EBTQuestState QuestState = QuestSubSystem->GetQuestState(QuestAsset);
				if(NewState == "Inactive")
				{
					if(QuestState != EBTQuestState::Inactive)
					{
						QuestSubSystem->AbandonQuest(QuestAsset);
					}
				}
				else if (NewState == "InProgress")
				{
					if(QuestState == EBTQuestState::Completed || QuestState == EBTQuestState::Failed)
					{
						//In case the player has already completed or failed the quest
						QuestSubSystem->AbandonQuest(QuestAsset);
					}
					if(QuestState != EBTQuestState::InProgress)
					{
						QuestSubSystem->AcceptQuest(QuestAsset, true);
					}
				}
				else if (NewState == "Completed")
				{
					if(QuestState != EBTQuestState::Completed)
					{
						QuestSubSystem->CompleteQuest(QuestAsset, true, true);
					}
				}
				else if(NewState == "Failed")
				{
					if(QuestState == EBTQuestState::Inactive)
					{
						QuestSubSystem->AcceptQuest(QuestAsset);
					}
					if(QuestState != EBTQuestState::Failed)
					{
						QuestSubSystem->FailQuest(QuestAsset, true);
					}
				}
				else
				{
					UE_LOG(LogQuestSystem, Log, TEXT("Invalid state passed into SetQuestState"))
				}
			}));
    
		// Alternatively, you can use the Streamable Manager directly:
		// UAssetManager::GetStreamableManager().RequestAsyncLoad(AssetPath, FStreamableDelegate::CreateLambda([...]));
	}
	else
	{
		UE_LOG(LogQuestSystem, Warning, TEXT("No asset matching '%s' was found!"), *PartialQuestName);
	}
}
