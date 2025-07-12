// Copyright (C) Varian Daemon 2025. All Rights Reserved.


#include "DialogueObjects/DialogueCondition.h"

bool UDialogueCondition::IsConditionMet_Implementation()
{
	return true;
}

UWorld* UDialogueCondition::GetWorld() const
{
	return GEngine->GetCurrentPlayWorld();
}
