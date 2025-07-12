// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "QuestCheatExtension.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable)
class BT_QUESTS_API UQuestCheatExtension : public UCheatManagerExtension
{
	GENERATED_BODY()

	UQuestCheatExtension();

	/**Forcefully set the state of a quest.
	 * @PartialQuestName Full or partial name of the quest asset, NOT the in-game name.
	 * @NewState The new state of the quest. (Inactive, Locked, InProgress, Completed, Failed)
	 *
	 * Use with caution, changing a quest from Completed to Inactive aren't scenarios that
	 * QA might be testing and most likely shouldn't be testing. */
	UFUNCTION(Exec)
	void SetQuestState(const FString& PartialQuestName, const FString& NewState);
};
