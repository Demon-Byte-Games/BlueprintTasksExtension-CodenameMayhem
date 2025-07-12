// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestTaskNode.h"
#include "MonitorQuestTask.generated.h"

/**
 * Monitors the state of an assigned quest.
 * Does NOT accept the quest. This is primarily
 * used for UI. */
UCLASS()
class BT_QUESTS_API UMonitorQuestTask : public UQuestTaskNode
{
	GENERATED_BODY()

public:
	
	UMonitorQuestTask(const FObjectInitializer& ObjectInitializer);
};
