// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestChain.generated.h"

class UQuestAsset;

USTRUCT(Blueprintable)
struct FBTQuestChainStage
{
	GENERATED_BODY()

	UPROPERTY(Category = "Quest Chain stage", BlueprintReadWrite, EditAnywhere)
	TArray<TSoftObjectPtr<UQuestAsset>> Quests;
};

/**
 * An asset that manages a set of quests, creating a "quest chain"
 */
UCLASS(meta=(ContextMenuCategory = "Varian's Plugins", ContextMenuEntryName = "Quests|Quest Chain", ContextMenuPrefix = "QC_"))
class BT_QUESTS_API UQuestChain : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Quest Chain", BlueprintReadOnly, EditAnywhere)
	FText ChainName;
	
	UPROPERTY(Category = "Quest Chain", BlueprintReadOnly, EditAnywhere)
	TArray<FBTQuestChainStage> Stages;

#if WITH_EDITOR

	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif
};
