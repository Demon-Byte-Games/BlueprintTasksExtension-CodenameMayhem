// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/QuestAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSystem.generated.h"

class UQuestAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestCompleted, FBTQuestWrapper, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestAbandoned, FBTQuestWrapper, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestFailed, FBTQuestWrapper, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestAccepted, FBTQuestWrapper, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestChainStarted, TSoftObjectPtr<UQuestAsset>, QuestChain);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FObjectiveProgressed, FQuestObjective, Objective, float, ProgressMade, bool, Finished, UObject*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectiveFailed, FQuestObjective, Objective);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQuestObjectiveStageCompleted, FQuestObjectiveStage, CompletedStage, FQuestObjectiveStage, NewStage);

/**
 * 
 */
UCLASS(DisplayName = "Quest System")
class BT_QUESTS_API UQuestSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UQuestSystem();

public:

	UPROPERTY(Category = "Quest System", BlueprintReadOnly, SaveGame)
	TMap<TSoftObjectPtr<UQuestAsset>, FBTQuestWrapper> Quests;

	UPROPERTY(Category = "Quest System", EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<UQuestChain>> QuestChains;

//-------------------------
#pragma region Delegates
	UPROPERTY(Category = "Quest System", BlueprintAssignable)
	FQuestCompleted QuestCompleted;
 
	UPROPERTY(Category = "Quest System", BlueprintAssignable)
	FQuestAbandoned QuestAbandoned;

	UPROPERTY(Category = "Quest System", BlueprintAssignable)
	FQuestFailed QuestFailed;

	UPROPERTY(Category = "Quest System", BlueprintAssignable)
	FQuestAccepted QuestAccepted;

	UPROPERTY(Category = "Quest System|Quest Chain", BlueprintAssignable)
	FQuestChainStarted QuestChainStarted;

	UPROPERTY(Category = "Quest System|Task", BlueprintAssignable)
	FObjectiveProgressed ObjectiveProgressed;

	UPROPERTY(Category = "Quest System|Task", BlueprintAssignable)
	FObjectiveFailed ObjectiveFailed;

	UPROPERTY(Category = "Quest System|Task", BlueprintAssignable)
	FQuestObjectiveStageCompleted QuestObjectiveStageCompleted;
#pragma endregion

	static UQuestSystem* Get();

//-------------------------
#pragma region Quest
	
	/**Accept a quest from a node.
	 * Will only return true if the quest was accepted,
	 * if it returns false it means the player has already
	 * completed it or has it.
	 * @ForceAccept if true, we skip CanAcceptQuest() */
	UFUNCTION(Category = "Quest System", BlueprintCallable)
	static bool AcceptQuest(TSoftObjectPtr<UQuestAsset> Quest, bool ForceAccept = false);

	UFUNCTION(Category = "Quest System", BlueprintPure)
	static bool CanAcceptQuest(TSoftObjectPtr<UQuestAsset> Quest);
	
	/**Complete the quest.
	 *
	 * @SkipCompletionCheck Typically, you want CanCompleteQuest to be called,
	 * but sometimes you want to forcibly complete the quest.
	 * @AutoAcceptQuest If true, we will accept the quest (forcefully) if the
	 * quest hasn't already been accepted.*/
	UFUNCTION(Category = "Quest", BlueprintCallable)
	static void CompleteQuest(TSoftObjectPtr<UQuestAsset> Quest, bool SkipCompletionCheck = false, bool AutoAcceptQuest = true);
	
	UFUNCTION(Category = "Quest System", BlueprintPure)
	static bool CanCompleteQuest(TSoftObjectPtr<UQuestAsset> Quest);
	bool CanCompleteQuest(FBTQuestWrapper Quest);
	
	UFUNCTION(Category = "Quest System", BlueprintPure)
	static EBTQuestState GetQuestState(TSoftObjectPtr<UQuestAsset> Quest);

	/**Abandon the quest, allowing it to be accepted again.*/
	UFUNCTION(Category = "Quest System", BlueprintCallable)
	static bool AbandonQuest(TSoftObjectPtr<UQuestAsset> Quest);
	
	/**Attempt to fail the quest, only returns false if the quest is
	 * not in progress.
	 *
	 * @FailTasks Whether or not the tasks should be labelled as "failed" */
	UFUNCTION(Category = "Quest System", BlueprintCallable)
	static bool FailQuest(TSoftObjectPtr<UQuestAsset> Quest, bool FailObjectives);

	/**Helper function for retrieving all quests with a specific state,
	 * useful for sorting a quest journal. */
	UFUNCTION(Category = "Quest System", BlueprintCallable)
	static TArray<FBTQuestWrapper> GetQuestsWithState(EBTQuestState State);

#pragma endregion
	
	
//-------------------------
#pragma region Quest Chain

	UFUNCTION(Category = "Quest System|Quest Chain", BlueprintPure)
	static TArray<TSoftObjectPtr<UQuestAsset>> GetRequiredQuestsForQuest(TSoftObjectPtr<UQuestAsset> Quest);
	
	/**Resolve whether the required quests have been completed for the @Quest.
	* Keep in mind, this will load the quest synchronously.*/
	UFUNCTION(Category = "Quest System|Quest Chain", BlueprintPure)
	static bool HasCompletedRequiredQuests(TSoftObjectPtr<UQuestAsset> Quest);

#pragma endregion

//-------------------------
#pragma region Objectives

	/**Search the active quests for the Objective.*/
	UFUNCTION(Category = "Quest System|Objective", BlueprintCallable)
	static FBTQuestWrapper GetQuestForObjective(FGameplayTag Objective);

	UFUNCTION(Category = "Quest System|Objective", BlueprintCallable)
	static FQuestObjective GetObjectiveByID(FGameplayTag ObjectiveID);

	UFUNCTION(Category = "Quest System|Objective", BlueprintPure)
	static EBTQuestState GetObjectiveState(FGameplayTag Objective);

	UFUNCTION(Category = "Quest System|Objective", BlueprintCallable)
	static bool CompleteObjective(FGameplayTag ObjectiveID, UObject* Instigator);

	/**Add or deduct progress from an objective.
	* @Instigator Who is attempting to progress the objective?*/
	UFUNCTION(Category = "Quest System|Objective", BlueprintCallable, meta = (DefaultToSelf = "Instigator"))
	static bool ProgressObjective(const FGameplayTag ObjectiveID, float ProgressToAdd, UObject* Instigator);

	/**Evaluate if the task can be progressed. */
	UFUNCTION(Category = "Quest System|Objective", BlueprintCallable)
	static bool CanObjectiveBeProgressed(FQuestObjective Objective);

	/**Attempt to fail a task.
	 *
	 * @FailQuest Whether the entire quest this task belongs to
	 * should also be failed.*/
	UFUNCTION(Category = "Quest System|Objective", BlueprintCallable)
	static bool FailObjective(FGameplayTag Objective, bool bFailQuest);

	
#pragma endregion
	
	
	/**Creates a wrapper for a quest so it can be accepted.
	 * The wrapper contains all mutable data revolving a quest. */
	UFUNCTION(Category = "Quest System|Helpers")
	static FBTQuestWrapper CreateQuestWrapper(TSoftObjectPtr<UQuestAsset> QuestAsset);
};

