// Copyright (C) Varian Daemon 2025. All Rights Reserved.


#include "Nodes/TaskGraphNode/GFA_StartTaskGraph.h"

#include "GameFeaturesSubsystem.h"
#include "Engine/AssetManager.h"
#include "Nodes/TaskGraphNode/TaskGraph.h"

void UGFA_StartTaskGraph::OnGameFeatureLoading()
{
	Super::OnGameFeatureLoading();

	for(auto& TaskGraph : TaskGraphsToActivate)
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(TaskGraph.ToSoftObjectPath());
	}
}

void UGFA_StartTaskGraph::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	Super::OnGameFeatureActivating(Context);

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if(Context.ShouldApplyToWorldContext(WorldContext))
		{
			for(auto& TaskGraph : TaskGraphsToActivate)
			{
				Streamable.RequestAsyncLoad(TaskGraph.ToSoftObjectPath(), [this, WorldContext, TaskGraph]
				{
					UTaskGraph* NewTaskGraph = NewObject<UTaskGraph>(WorldContext.World(), TaskGraph.Get());
					NewTaskGraph->StartGraph();
				});
			}
		}
	}
}

void UGFA_StartTaskGraph::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	TRACE_CPUPROFILER_EVENT_SCOPE(TaskGraphGameFeatureDeactivate)
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if(Context.ShouldApplyToWorldContext(WorldContext))
		{
			/**TODO: Check if this is expensive
			 * I believe all actors outers is the world,
			 * so theoretically this would loop over ALL
			 * actors in the level. Need to check if this
			 * is expensive.
			 * if it is expensive, it might be worthwhile
			 * to create a subsystem and assign that as
			 * the outer for level-bound task graphs. */
			TArray<UObject*> SubObjects;
			GetObjectsWithOuter(WorldContext.World(), SubObjects);

			for(auto& CurrentObject : SubObjects)
			{
				if(UTaskGraph* TaskGraph = Cast<UTaskGraph>(CurrentObject))
				{
					TaskGraph->FinishGraph();
				}
			}
		}
	}
}
