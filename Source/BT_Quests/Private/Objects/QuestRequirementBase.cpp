// Copyright (C) Varian Daemon 2025. All Rights Reserved.


#include "Objects/QuestRequirementBase.h"

bool UQuestRequirementBase::IsConditionMet_Implementation(const TSoftObjectPtr<UQuestAsset>& Quest)
{
	return true;
}

UWorld* UQuestRequirementBase::GetWorld() const
{
	return GEngine->GetCurrentPlayWorld();
}
