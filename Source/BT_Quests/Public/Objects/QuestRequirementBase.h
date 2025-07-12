// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/QuestAsset.h"
#include "UObject/Object.h"
#include "QuestRequirementBase.generated.h"

class UQuestSystem;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, CollapseCategories,
	meta=(ContextMenuCategory = "Varian's Plugins", ContextMenuEntryName = "Quests|Quest Requirement", ContextMenuPrefix = "QR_"))
class BT_QUESTS_API UQuestRequirementBase : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	bool IsConditionMet(const TSoftObjectPtr<UQuestAsset>& Quest);

	virtual UWorld* GetWorld() const override;
};
