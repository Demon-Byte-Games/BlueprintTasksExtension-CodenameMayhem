// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"

#include "GFA_StartTaskGraph.generated.h"

class UTaskGraph;
/**
 * 
 */
UCLASS(DisplayName = "Start Task Graphs")
class BLUEPRINTTASKSEXTENSION_API UGFA_StartTaskGraph : public UGameFeatureAction
{
	GENERATED_BODY()

public:

	/**TaskGraph's to associate with the current world.
	 * These graphs outers will become the world, so if the
	 * world is ever changed, these graphs are destroyed. */
	UPROPERTY(EditAnywhere)
	TSet<TSoftClassPtr<UTaskGraph>> TaskGraphsToActivate;

	virtual void OnGameFeatureLoading() override;

	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;

	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
};
