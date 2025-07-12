// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NodeDecorators/BNTNodeDecorator.h"
#include "QuestTaskDecorator.generated.h"

class UQuestAsset;
/**
 * 
 */
UCLASS()
class BT_QUESTS_API UQuestTaskDecorator : public UBNTNodeDecorator
{
	GENERATED_BODY()
	
	TSharedPtr<SVerticalBox> CenterContentArea;
	TSharedPtr<STextBlock> CenterText;
	
	TWeakObjectPtr<UQuestAsset> QuestInformation = nullptr;

	virtual TSharedRef<SWidget> CreateCenterContent(UClass* TaskClass, UBlueprintTaskTemplate* BlueprintTaskNode, UEdGraphNode* GraphNode) override;

	virtual TArray<UObject*> GetObjectsForExtraDetailsPanels() const override;
};
