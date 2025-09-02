// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueTask.h"
#include "DataAssets/DialogueCharacter.h"
#include "NodeDecorators/BtfNodeDecorator.h"

#include "DialogueTaskDecorator.generated.h"

/**
 * 
 */
UCLASS()
class BT_DIALOGUE_API UDialogueTaskDecorator : public UBtf_NodeDecorator
{
	GENERATED_BODY()

	FSlateBrush Brush;

	UPROPERTY()
	UTexture2D* Portrait = nullptr;

	virtual void BeginDestroy() override
	{
		Super::BeginDestroy();
	}

	virtual TSharedRef<SWidget> CreateTopContent(UClass* TaskClass, UBtf_TaskForge* BlueprintTaskNode, UEdGraphNode* GraphNode) override
	{
		if(!Cast<UDialogueTask>(BlueprintTaskNode)->Script.Character.IsNull())
		{
			return SNew(STextBlock)
				.Text(Cast<UDialogueTask>(BlueprintTaskNode)->Script.Character.LoadSynchronous()->Name);
		}

		return Super::CreateTopContent(TaskClass, BlueprintTaskNode, GraphNode);
	}

	virtual TSharedRef<SWidget> CreateCenterContent(UClass* TaskClass, UBtf_TaskForge* BlueprintTaskNode, UEdGraphNode* GraphNode) override
	{
		Portrait = Cast<UDialogueTask>(BlueprintTaskNode)->GetSpeakerPortrait();
		if(Portrait)
		{
			Brush.SetImageSize(FVector2D(64));
			Brush.SetResourceObject(Portrait);	
		}
		else
		{
			Brush.SetImageSize(FVector2D(0));
			Brush.SetResourceObject(nullptr);
		}

		return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.AutoWidth()
		.Padding(0.f)
		[
			SNew(SImage)
			.Image(&Brush) //Portrait on the left
		]
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		.FillWidth(1.0f)
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Cast<UDialogueTask>(BlueprintTaskNode)->GetCenterText())) //Character dialogue
			.WrapTextAt(250.f)
		];
	}
	
	void HandleTextChanged(const FText& InText);
	void HandleTextCommitted(const FText& InText, ETextCommit::Type CommitType);
};
