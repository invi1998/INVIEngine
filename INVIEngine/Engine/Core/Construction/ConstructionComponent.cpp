#include "EngineMinimal.h"
#include "ConstructionComponent.h"
#include "Actor/Core/ActorObject.h"
#include "Component/TransformationComponent.h"
#include "Component/Core/Component.h"

namespace ConstructionComponent
{
	void ConstructionComponent(CCoreMinimalObject* InOwner, CCoreMinimalObject* NewObject)
	{
		if (CComponent* component = dynamic_cast<CComponent*>(NewObject))
		{
			if (InOwner)
			{
				// ���InOwner��һ��Component
				if (CComponent* comp = dynamic_cast<CComponent*>(InOwner))
				{
					UpdateComponent(component, comp);
				}
				// ���InOwner��һ��ActorObject
				else if (GActorObject* actor = dynamic_cast<GActorObject*>(InOwner))
				{
					if (actor->GetRootComponent())
					{
						UpdateComponent(component, actor->GetRootComponent());
					}
				}
			}
		}
	}

	void UpdateComponent(CComponent* component, CComponent* parentComponent)
	{
		if (component && parentComponent && component != parentComponent)
		{
			parentComponent->AddChildComponent(component);
		}
	}
}

