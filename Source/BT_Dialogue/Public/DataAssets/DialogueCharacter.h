// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueCharacter.generated.h"

/**
 * 
 */
UCLASS(meta=(ContextMenuCategory = "Varian's Plugins", ContextMenuEntryName = "Dialogue|Character", ContextMenuPrefix = "DC_"))
class BT_DIALOGUE_API UDialogueCharacter : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Visuals", BlueprintReadOnly, EditAnywhere)
	TSoftObjectPtr<UTexture2D> CharacterPortrait = nullptr;

	UPROPERTY(Category = "Visuals", BlueprintReadOnly, EditAnywhere)
	FText Name;

	
#if WITH_EDITORONLY_DATA

	/**Optional portrait used only for the node. Ideally, this would be
	 * a perfect square. */
	UPROPERTY(Category = "Developer Settings", BlueprintReadOnly, EditAnywhere, meta = (DevelopmentOnly, ThumbnailOverride))
	TSoftObjectPtr<UTexture2D> NodePortrait = nullptr;

	UPROPERTY(Category = "Developer Settings", BlueprintReadOnly, EditAnywhere, meta = (DevelopmentOnly))
	FLinearColor NodeColor;
	
#endif
};
