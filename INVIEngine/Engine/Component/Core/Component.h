#pragma once
#include "Core/CoreObject/CoreMinimalObject.h"

class CComponent : public CCoreMinimalObject
{
public:
	CComponent() = default;
	virtual ~CComponent() override;
	void BeginInit() override;
	void Tick(float DeltaTime) override;

	void AddChildComponent(CComponent* component);

	FORCEINLINE CComponent* GetParentComponents() const { return ParentComponent; }	// ���ظ����
	FORCEINLINE std::vector<CComponent*>& GetChildComponents() { return ChildComponents; }	// ����������б������

protected:
	CComponent* ParentComponent = nullptr;
	std::vector<CComponent*> ChildComponents;
};

