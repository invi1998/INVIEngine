#include "CoreObjectPch.h"

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
	if (component && component != this && component != ParentComponent)	// 防止循环引用, 防止重复添加
	{
		component->ParentComponent = this;		// 设置子组件的父组件为当前组件
		ChildComponents.push_back(component);	// 将子组件添加到当前组件的子组件列表中
	}
}

