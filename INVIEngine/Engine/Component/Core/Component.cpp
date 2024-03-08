#include "EngineMinimal.h"

#include "Component.h"

CComponent::~CComponent()
{
}

void CComponent::BeginInit()
{
	CCoreMinimalObject::BeginInit();
}

void CComponent::Tick(float DeltaTime)
{
	CCoreMinimalObject::Tick(DeltaTime);
}

void CComponent::AddChildComponent(CComponent* component)
{
	if (component && component != this)
	{
		component->ParentComponent = this;
		ChildComponents.push_back(component);
	}
}
