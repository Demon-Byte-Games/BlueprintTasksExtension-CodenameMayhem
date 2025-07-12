#include "CogTasksTracker.h"

#include "BlueprintTaskTemplate.h"
#include "Nodes/TaskGraphNode/TaskGraph.h"
#include "Subsystem/BlueprintTaskTrackerSubsystem.h"

#if COG_INSTALLED && ENABLE_COG

void FCogTasksTracker::RenderHelp()
{
	ImGui::Text(
	"Display all active tasks and their owners in a tree"
	);
}

void FCogTasksTracker::RenderContent()
{
	UBlueprintTaskTrackerSubsystem* TrackerSubsystem = GEngine->GetEngineSubsystem<UBlueprintTaskTrackerSubsystem>();
	if(!TrackerSubsystem)
	{
		return;
	}

	HandledObjects.Empty();

	for(auto& CurrentObject : TrackerSubsystem->ObjectsAndTheirTasks)
	{
		if(CurrentObject.Key.IsValid())
		{
			if(HandledObjects.Contains(CurrentObject.Key.Get()))
			{
				continue;
			}

			/**The map might not be arranged in a way that allows us to render the tree neatly
			 * with no duplicates.
			 * This setup will climb up the outer chain, ask that outer if it has any tasks,
			 * if true, that means this object is inside of another object, probably a task
			 * inside of another task.
			 * In that case, we don't want to place the CurrentObject at the root of the tree,
			 * we want the parent outer. */
			UObject* TargetObject = CurrentObject.Key.Get();
			for(UObject* Outer = CurrentObject.Key.Get()->GetOuter(); Outer; Outer = Outer->GetOuter())
			{
				TArray<UObject*> Children;
				GetObjectsWithOuter(Outer, Children, false);
				for(auto& CurrentChild : Children)
				{
					if(CurrentChild->IsA(UBlueprintTaskTemplate::StaticClass())
						|| CurrentChild->IsA(UTaskGraph::StaticClass())
						|| TrackerSubsystem->ObjectsAndTheirTasks.Contains(Outer)
						&& CurrentChild != CurrentObject.Key.Get())
					{
						TargetObject = Outer;
					}
				}
			}

			/**Prevent duplicates*/
			if(HandledObjects.Contains(TargetObject))
			{
				continue;
			}
			
			RenderObjectTree(TargetObject);
			ImGui::Separator();
		}
	}
}

void FCogTasksTracker::RenderObjectTree(UObject* Object)
{
	if (!Object)
		return;

	HandledObjects.Add(Object);

	if(UBlueprintTaskTemplate* TaskObject = Cast<UBlueprintTaskTemplate>(Object))
	{
		if(!TaskObject->Get_IsActive())
		{
			return;
		}
	}

	//Retrieve children for the current object.
	TArray<UObject*> Children;
	GetObjectsWithOuter(Object, Children, false);

	//Use a leaf flag if there are no children, so it doesn't show an arrow.
	ImGuiTreeNodeFlags flags = Children.Num() == 0 
		? ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen 
		: 0;

	//Create the tree node; use a unique identifier if needed.
	bool nodeOpen = ImGui::TreeNodeEx(TCHAR_TO_ANSI(*Object->GetName()), flags);
    
	//If there are children and the node is expanded, draw them recursively.
	if (nodeOpen && Children.Num() > 0)
	{
		for(UObject* Child : Children)
		{
			if(HandledObjects.Contains(Child))
			{
				continue;
			}
			
			if(Child->IsA(UBlueprintTaskTemplate::StaticClass())
				|| Child->IsA(UTaskGraph::StaticClass()))
			{
				RenderObjectTree(Child);
				continue;
			}

			TArray<UObject*> TempChildren;
			GetObjectsWithOuter(Child, TempChildren, false);
			for(auto& CurrentTempChild : TempChildren)
			{
				if(CurrentTempChild->IsA(UBlueprintTaskTemplate::StaticClass())
					|| CurrentTempChild->IsA(UTaskGraph::StaticClass()))
				{
					RenderObjectTree(Child);
				}
			}
		}
		ImGui::TreePop();
	}
}

#endif
