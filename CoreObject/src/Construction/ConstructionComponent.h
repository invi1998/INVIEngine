#pragma once


class CCoreMinimalObject;
class CComponent;

namespace ConstructionComponent
{
	// 构造对象组件
	void ConstructionComponent(CCoreMinimalObject* InOwner, CCoreMinimalObject* NewObject);

	void UpdateComponent(CComponent* component, CComponent* parentComponent);
}




