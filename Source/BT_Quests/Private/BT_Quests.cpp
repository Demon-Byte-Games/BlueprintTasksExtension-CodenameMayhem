#include "BT_Quests.h"

#define LOCTEXT_NAMESPACE "FBT_QuestsModule"

DEFINE_LOG_CATEGORY(LogQuestSystem);

void FBT_QuestsModule::StartupModule()
{
	UE_LOG(LogQuestSystem, Log, TEXT("Quest system module initialized"))
}

void FBT_QuestsModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FBT_QuestsModule, BT_Quests)