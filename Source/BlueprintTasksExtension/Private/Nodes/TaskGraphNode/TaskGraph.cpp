// Copyright (C) Varian Daemon 2025. All Rights Reserved.


#include "Nodes/TaskGraphNode/TaskGraph.h"

AActor* UTaskGraph::GetOwningActor() const
{
	return GetTypedOuter<AActor>();
}

bool UTaskGraph::IsTickable() const
{
	return true;
}

TStatId UTaskGraph::GetStatId() const
{
	return TStatId();
}

class UWorld* UTaskGraph::GetWorld() const
{
	if (IsTemplate() || !GetOuter()) // We're the CDO or have no outer (?!).
	{
		return nullptr;
	}
	
	for(UObject* Outer = GetOuter(); Outer; Outer = Outer->GetOuter())
	{
		if(Outer && Outer->GetWorld())
		{
			return Outer->GetWorld();
		}
	}

	return GetOuter()->GetWorld();
}
