// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BtfTaskForge.h"
#include "DataAssets/QuestAsset.h"

#include "QuestTaskNode.generated.h"

struct FQuestObjectiveStage;
class UQuestObjectiveRequirement;
class UQuestAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestAccepted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestFailed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestAbandoned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestFailedRequirements);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestObjectiveCompleted, FQuestObjective, CompletedObjective);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestObjectiveStageCompleted, FQuestObjectiveStage, CompletedStage, FQuestObjectiveStage, NewStage);

/**
 * A node responsible for starting and tracking a quest.
 */
UCLASS()
class BT_QUESTS_API UQuestTaskNode : public UBtf_TaskForge
{
	GENERATED_BODY()

public:

	UQuestTaskNode(const FObjectInitializer& ObjectInitializer);

	virtual void Activate_Internal() override;

	bool AcceptQuestOnActivate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<UQuestAsset> QuestAsset = nullptr;

	/**If this node is triggered and the quest is not inactive,
	 * for example if it's already completed, we will automatically trigger
	 * the completed pin. The same applies to the other pins. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	bool TriggerStatePinsIfQuestIsNotInactive = true;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnQuestAccepted QuestAccepted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnQuestCompleted QuestCompleted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnQuestObjectiveStageCompleted ObjectiveStageCompleted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnQuestFailed QuestFailed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnQuestAbandoned QuestAbandoned;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnQuestFailedRequirements QuestFailedRequirements;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnQuestObjectiveCompleted QuestObjectiveCompleted;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnQuestObjectiveStageCompleted QuestObjectiveStageCompleted;

	virtual bool Get_NodeTitleColor_Implementation(FLinearColor& Color) override;

	UFUNCTION()
	void OnQuestCompleted(FBTQuestWrapper Quest)
	{
		if(Quest.QuestAsset == QuestAsset)
		{
			QuestCompleted.Broadcast();
			Deactivate();
		}
	}

	UFUNCTION()
	void OnQuestAbandoned(FBTQuestWrapper Quest)
	{
		if(Quest.QuestAsset == QuestAsset)
		{
			QuestAbandoned.Broadcast();
			Deactivate();
		}
	}

	UFUNCTION()
	void OnQuestFailed(FBTQuestWrapper Quest)
	{
		if(Quest.QuestAsset == QuestAsset)
		{
			QuestFailed.Broadcast();
			Deactivate();
		}
	}

	UFUNCTION()
	void OnQuestAccepted(FBTQuestWrapper Quest)
	{
		if(Quest.QuestAsset == QuestAsset)
		{
			QuestAccepted.Broadcast();
		}
	}
	
	UFUNCTION()
	void OnQuestObjectiveCompleted(FQuestObjective Objective, float ProgressMade, bool Finished, UObject* Instigator)
	{
		if(Objective.RootQuest == QuestAsset && Finished)
		{
			QuestObjectiveCompleted.Broadcast(Objective);
		}
	}

	UFUNCTION()
	void OnQuestObjectiveStageCompleted(FQuestObjectiveStage CompletedStage, FQuestObjectiveStage NewStage)
	{
		if(CompletedStage.Objectives[0].RootQuest == QuestAsset)
		{
			QuestObjectiveStageCompleted.Broadcast(CompletedStage, NewStage);
		}
	}
};
