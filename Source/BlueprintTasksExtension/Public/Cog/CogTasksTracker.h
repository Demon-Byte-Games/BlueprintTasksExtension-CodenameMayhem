#pragma once

#if COG_INSTALLED

#include "CogCommon.h"
#if ENABLE_COG

#include "CogWindow.h"

class BLUEPRINTTASKSEXTENSION_API FCogTasksTracker : public FCogWindow
{
public:

	TSet<TObjectPtr<UObject>> HandledObjects; 

	virtual void RenderHelp() override;

	virtual void RenderContent() override;

	void RenderObjectTree(UObject* Object);
};

#endif
#endif