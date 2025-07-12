// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TaskGraph.h"
#include "Engine/CancellableAsyncAction.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActivateTaskGraph.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadAndStartTaskGraph, FGameplayTagContainer, FinishReasons);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskGraphFinished, FGameplayTagContainer, FinishReasons);

/**
 * 
 */
UCLASS()
class BLUEPRINTTASKSEXTENSION_API UAsyncStartTask : public UCancellableAsyncAction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FLoadAndStartTaskGraph GraphStarted;

	UPROPERTY(BlueprintAssignable)
	FTaskGraphFinished GraphFinished;

	UPROPERTY()
	TObjectPtr<UObject> InOuter = nullptr;
	
	TSoftClassPtr<UTaskGraph> InTaskGraph = nullptr;

	UPROPERTY()
	TObjectPtr<UTaskGraph> SpawnedTaskGraph = nullptr;

	/**Async load and start a task graph.
	 * If you want to manually deactivate it,
	 * cancel the returned task object. */
	UFUNCTION(Category="Task Graph", BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="Outer", DefaultToSelf = "Outer"))
	static UAsyncStartTask* AsyncStartTaskGraph(UObject* Outer, TSoftClassPtr<UTaskGraph> TaskGraph);

	virtual void Activate() override;

	virtual void Cancel() override;

	UFUNCTION()
	void OnTaskFinished(FGameplayTagContainer FinishResponse);
};
