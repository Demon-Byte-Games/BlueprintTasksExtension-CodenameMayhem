// Copyright (C) Varian Daemon 2025. All Rights Reserved.


#include "Nodes/TaskGraphNode/AsyncActivateTaskGraph.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Nodes/TaskGraphNode/TaskGraph.h"

UAsyncStartTask* UAsyncStartTask::AsyncStartTaskGraph(UObject* Outer, TSoftClassPtr<UTaskGraph> TaskGraph)
{
	UAsyncStartTask* NewTask = NewObject<UAsyncStartTask>(Outer);
	NewTask->InOuter = Outer;
	NewTask->InTaskGraph = TaskGraph;
	return NewTask;
}

void UAsyncStartTask::Activate()
{
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	
	Streamable.RequestAsyncLoad(InTaskGraph.ToSoftObjectPath(), [this]
	{
		UTaskGraph* TaskGraph = NewObject<UTaskGraph>(InOuter, InTaskGraph.Get());
		TaskGraph->GraphFinished.AddDynamic(this, &UAsyncStartTask::OnTaskFinished);
		
		TaskGraph->StartGraph();
		GraphStarted.Broadcast(FGameplayTagContainer());
	});
}

void UAsyncStartTask::Cancel()
{
	if(SpawnedTaskGraph)
	{
		SpawnedTaskGraph->FinishGraph();
	}
	
	Super::Cancel();
}

void UAsyncStartTask::OnTaskFinished(FGameplayTagContainer FinishReasons)
{
	GraphFinished.Broadcast(FinishReasons);
}
