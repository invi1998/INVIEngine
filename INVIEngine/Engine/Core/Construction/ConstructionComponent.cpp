#include "EngineMinimal.h"
#include "ConstructionComponent.h"

void ConstructionComponent::ConstructionComponent(GActorObject* InOwner, CCoreMinimalObject* NewObject)
{
	if (CComponent* component = dynamic_cast<CComponent*>(NewObject))
	{
		if (InOwner)
		{
			// 如果InOwner是一个ActorObject
			if (GActorObject* actor = dynamic_cast<GActorObject*>(InOwner))
			{
				if (actor->GetRootComponent())
				{
					actor->GetRootComponent()->AddChildComponent(component);
				}
			}
			else if (CComponent* comp = dynamic_cast<CComponent*>(InOwner))
			{
				comp->AddChildComponent(component);
			}
		}
	}
}
