#include "EngineMinimal.h"

#include "Component.h"

CComponent::~CComponent()
{
	ChildComponents.clear();
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
		ChildComponents.push_back(component);
	}
}

void CComponent::SetParentComponent(CComponent* component)
{
	if (component && component != this)
	{
		ParentComponent = component;
	}
}
