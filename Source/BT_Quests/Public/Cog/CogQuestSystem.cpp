#include "CogQuestSystem.h"

#if COG_INSTALLED && ENABLE_COG

#include "CogWindowWidgets.h"
#include "QuestSystem.h"
#include "DataAssets/QuestAsset.h"
#include "DataAssets/QuestChain.h"
#include "Engine/AssetManager.h"
#include "Kismet/KismetStringLibrary.h"

void FCogQuestSystem::RenderHelp()
{
	ImGui::Text(
	"Debugger for managing BTE's quest system"
	);
}

void FCogQuestSystem::RenderContent()
{
	UQuestSystem* QuestSubSystem = UQuestSystem::Get();
	if(!QuestSubSystem)
	{
		ImGui::Text("No quest subsystem found");
	}

	///////////////
	// Quest chains
	//////////////
	ImGui::PushID("Quest Chain");
	if(ImGui::TreeNodeEx("Quest Chains"))
	{
		for(auto& CurrentChain : QuestSubSystem->QuestChains)
		{
			if(ImGui::TreeNodeEx(TCHAR_TO_ANSI(*CurrentChain.LoadSynchronous()->ChainName.ToString())))
			{
				int32 StageIndex = 0;
				for(auto& CurrentStage : CurrentChain.Get()->Stages)
				{
					if(ImGui::TreeNodeEx(TCHAR_TO_ANSI(*FString("Stage " + FString::FromInt(StageIndex)))))
					{
						for(auto& CurrentQuest : CurrentStage.Quests)
						{
							/**Label at the top of the buttons and table*/
							FString QuestLabel = CurrentQuest.LoadSynchronous()->QuestName.ToString()
							+ " - "
							+ StaticEnum<EBTQuestState>()->GetNameStringByIndex(static_cast<int32>(QuestSubSystem->GetQuestState(CurrentQuest)))
							+ " - "
							+ CurrentQuest->QuestID.ToString();

							if(ImGui::TreeNodeEx(TCHAR_TO_ANSI(*QuestLabel)))
							{
								/**Complete and fail buttons*/
								if(ImGui::Button("Complete Quest"))
								{
									QuestSubSystem->CompleteQuest(CurrentQuest, true, true);
								}
								ImGui::SameLine();
								if(ImGui::Button("Fail Quest"))
								{
									QuestSubSystem->FailQuest(CurrentQuest, true);
								}

								FBTQuestWrapper* QuestWrapper = QuestSubSystem->Quests.Find(CurrentQuest);
								CreateTableForQuest(QuestWrapper, QuestSubSystem);
							}
						}
						ImGui::TreePop();
					}
					StageIndex++;
				}

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
	ImGui::PopID();
	
	///////////////
	// Active Quests
	//////////////
	ImGui::PushID("Active Quests");
	if(ImGui::TreeNodeEx("Active Quests"))
	{
		for(auto& CurrentQuest : QuestSubSystem->GetQuestsWithState(EBTQuestState::InProgress))
		{
			if(CurrentQuest.QuestAsset.LoadSynchronous()->QuestChains.IsValidIndex(0))
			{
				//Quest chains window should handle this quest
				continue;
			}

			FBTQuestWrapper* QuestWrapper = QuestSubSystem->Quests.Find(CurrentQuest.QuestAsset);
			
			FString QuestLabel = CurrentQuest.QuestAsset.LoadSynchronous()->QuestName.ToString()
			+ " - "	+ CurrentQuest.QuestAsset->QuestID.ToString();
			if(ImGui::TreeNodeEx(TCHAR_TO_ANSI(*QuestLabel)))
			{
				/**Complete and fail buttons*/
				if(ImGui::Button("Complete Quest"))
				{
					QuestSubSystem->CompleteQuest(CurrentQuest.QuestAsset, true, true);
				}
				ImGui::SameLine();
				if(ImGui::Button("Fail Quest"))
				{
					QuestSubSystem->FailQuest(CurrentQuest.QuestAsset, true);
				}
					
				CreateTableForQuest(QuestWrapper, QuestSubSystem);

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
	ImGui::PopID();

	ImGui::PushID("Non-Active Quests");
	if(ImGui::CollapsingHeader("Non-Active Quests"))
	{
		ImGui::Separator();
		ImGui::Text("Non-Active quests");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - FCogWindowWidgets::GetFontWidth() * 3.0f);
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(29, 42, 62, 240));
			const float HelpWidth = FCogWindowWidgets::GetFontWidth() * 80;
			ImGui::SetNextWindowSizeConstraints(ImVec2(HelpWidth / 2.0f, 0.0f), ImVec2(HelpWidth, FLT_MAX));
			if (ImGui::BeginTooltip())
			{
				ImGui::PushTextWrapPos(HelpWidth - 1 * FCogWindowWidgets::GetFontWidth());
				ImGui::Text("Accepting any of these quests will instantly"
				   "load the quest and all related assets.");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
			ImGui::PopStyleColor();
		}
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(FCogWindowWidgets::GetFontWidth() * 1, 0));
		// Get the Asset Registry
		UAssetManager& AssetManager = UAssetManager::Get();

		// Retrieve the list of Primary Asset IDs for the specified type
		TArray<FPrimaryAssetId> PrimaryAssetIdList;
		AssetManager.GetPrimaryAssetIdList("Quests", PrimaryAssetIdList);

		// Iterate over the Primary Asset IDs and print their names
		for(const FPrimaryAssetId& AssetId : PrimaryAssetIdList)
		{
			TSoftObjectPtr<UQuestAsset> Quest = TSoftObjectPtr<UQuestAsset>(AssetManager.GetPrimaryAssetPath(AssetId));
			if(QuestSubSystem->Quests.Contains(Quest))
			{
				//Quest has been interacted with in some way
				continue;
			}
			if(ImGui::CollapsingHeader(TCHAR_TO_ANSI(*AssetId.PrimaryAssetName.ToString())))
			{
				if(ImGui::Button("Accept Quest"))
				{
					QuestSubSystem->AcceptQuest(Quest, true);
				}
			}
		}
	}
	ImGui::PopID();

	ImGui::PushID("Completed Quests");
	if(ImGui::CollapsingHeader("Completed Quests"))
	{
		for(auto& Quest : QuestSubSystem->GetQuestsWithState(EBTQuestState::Completed))
		{
			if(ImGui::TreeNodeEx(TCHAR_TO_ANSI(*Quest.QuestAsset.LoadSynchronous()->QuestName.ToString())))
			{
				for(int32 StageIndex = 0; StageIndex < Quest.ObjectiveStages.Num(); StageIndex++)
				{
					ImGui::Text(TCHAR_TO_ANSI(*FString("Stage: " + FString::FromInt(StageIndex))));
					for(auto& CurrentObjective : Quest.ObjectiveStages[StageIndex].Objectives)
					{
						ImGui::Text(TCHAR_TO_ANSI(*FString(
							CurrentObjective.ObjectiveName.ToString() + " "
							+ FString::SanitizeFloat(CurrentObjective.CurrentProgress)
							+ "/"
							+ FString::SanitizeFloat(CurrentObjective.ProgressRequired)
							+ " (" + StaticEnum<EBTQuestState>()->GetNameStringByIndex(static_cast<int32>(CurrentObjective.State)) + ")")));
					}
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::PopID();
	
	// ImGui::PushID("Failed Quests");
	// if(ImGui::CollapsingHeader("Failed Quests"))
	// {
	// 	for(auto& Quest : QuestSubSystem->GetQuestsWithState(EBTQuestState::Completed))
	// 	{
	// 		if(ImGui::CollapsingHeader(TCHAR_TO_ANSI(*Quest.QuestAsset.LoadSynchronous()->QuestName.ToString())))
	// 		{
	// 			for(int32 StageIndex = 0; StageIndex < Quest.ObjectiveStages.Num(); StageIndex++)
	// 			{
	// 				ImGui::Text(TCHAR_TO_ANSI(*FString("Stage: " + FString::FromInt(StageIndex))));
	// 				for(auto& CurrentObjective : Quest.ObjectiveStages[StageIndex].Objectives)
	// 				{
	// 					ImGui::Text(TCHAR_TO_ANSI(*FString(
	// 						CurrentObjective.ObjectiveName.ToString()
	// 						+ FString::SanitizeFloat(CurrentObjective.CurrentProgress)
	// 						+ "/"
	// 						+ FString::SanitizeFloat(CurrentObjective.ProgressRequired))));
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	// ImGui::PopID();
}

void FCogQuestSystem::CreateTableForQuest(FBTQuestWrapper* QuestWrapper, UQuestSystem* QuestSystem)
{
	if(!QuestWrapper)
	{
		return;
	}

	int32 CurrentStageNumber = 0;
	for(auto& CurrentStage : QuestWrapper->ObjectiveStages)
	{
		CurrentStageNumber++;
		//Use a leaf flag if there are no children, so it doesn't show an arrow.
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		//Create the tree node; use a unique identifier if needed.
		if(ImGui::TreeNodeEx(TCHAR_TO_ANSI(*FString("Stage: " + FString::FromInt(CurrentStageNumber))), flags))
		{
			if(ImGui::BeginTable("QuestsObjectives", 5,
			  ImGuiTableFlags_Resizable
			| ImGuiTableFlags_Reorderable
			| ImGuiTableFlags_Hideable
			| ImGuiTableFlags_SortMulti
			| ImGuiTableFlags_RowBg
			| ImGuiTableFlags_Borders
			| ImGuiTableFlags_NoBordersInBody
			| ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::TableSetupScrollFreeze(0, 1);
				ImGui::TableSetupColumn("Objective");
				ImGui::TableSetupColumn("Progress");
				ImGui::TableSetupColumn("State");
				ImGui::TableSetupColumn("Is Optional");
				ImGui::TableSetupColumn("");
				ImGui::TableHeadersRow();

				for(auto& CurrentObjective : CurrentStage.Objectives)
				{
					ImGui::PushID(TCHAR_TO_ANSI(*CurrentObjective.ObjectiveID.ToString()));
					//Objective name
					ImGui::TableNextColumn();
					ImGui::Text(TCHAR_TO_ANSI(*CurrentObjective.ObjectiveName.ToString()));

					//Progress
					ImGui::TableNextColumn();
					FString Progress =
						FString::SanitizeFloat(CurrentObjective.CurrentProgress)
						+ " / "											
						+ FString::SanitizeFloat(CurrentObjective.ProgressRequired);
					ImGui::Text(TCHAR_TO_ANSI(*Progress));

					//State
					ImGui::TableNextColumn();
					ImGui::Text(TCHAR_TO_ANSI(*StaticEnum<EBTQuestState>()->GetNameStringByIndex(static_cast<int32>(CurrentObjective.State))));

					//Is Optional
					ImGui::TableNextColumn();
					ImGui::Text(TCHAR_TO_ANSI(*UKismetStringLibrary::Conv_BoolToString(CurrentObjective.IsOptional)));

					//Buttons
					ImGui::TableNextColumn();
					if(ImGui::Button("Complete"))
					{
						QuestSystem->CompleteObjective(CurrentObjective.ObjectiveID, nullptr);
					}
					ImGui::SameLine();
					if(ImGui::Button("Fail"))
					{
						QuestSystem->FailObjective(CurrentObjective.ObjectiveID, false);
					}
					ImGui::PopID();
				}
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
	}
}

#endif