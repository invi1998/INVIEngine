#pragma once


class CCoreMinimalObject;
class CComponent;

namespace ConstructionComponent
{
	// ����������
	void ConstructionComponent(CCoreMinimalObject* InOwner, CCoreMinimalObject* NewObject);

	void UpdateComponent(CComponent* component, CComponent* parentComponent);
}




