#pragma once

#if COG_INSTALLED

#include "CogCommon.h"
#if ENABLE_COG

#include "CogWindow.h"

class UQuestSystem;
struct FBTQuestWrapper;

class BT_QUESTS_API FCogQuestSystem : public FCogWindow
{
public:

	virtual void RenderHelp() override;

	virtual void RenderContent() override;

	void CreateTableForQuest(FBTQuestWrapper* QuestWrapper, UQuestSystem* QuestSystem);
	
	TArray<FAssetData> AssetDataList;
	bool SearchedAssets = false;
};

#endif
#endif