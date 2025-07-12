// Copyright (C) Varian Daemon 2025. All Rights Reserved.


#include "DataAssets/QuestAsset.h"

FPrimaryAssetId UQuestAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FName("Quest Asset"), GetFName());
}
