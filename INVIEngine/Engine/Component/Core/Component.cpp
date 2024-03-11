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
	if (component && component != this && component != ParentComponent)	// ��ֹѭ������, ��ֹ�ظ����
	{
		component->ParentComponent = this;		// ����������ĸ����Ϊ��ǰ���
		ChildComponents.push_back(component);	// ���������ӵ���ǰ�����������б���
	}
}

