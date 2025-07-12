// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogueCondition.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, AutoExpandCategories = ("Default"), EditInlineNew,
	meta=(ContextMenuCategory = "Varian's Plugins", ContextMenuEntryName = "Dialogue|Condition", ContextMenuPrefix = "DC_"))
class BT_DIALOGUE_API UDialogueCondition : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(Category = "Dialogue", BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	bool IsConditionMet();

	virtual UWorld* GetWorld() const override;
};
