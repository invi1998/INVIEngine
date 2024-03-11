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

	FORCEINLINE CComponent* GetParentComponents() const { return ParentComponent; }	// 返回父组件
	FORCEINLINE std::vector<CComponent*>& GetChildComponents() { return ChildComponents; }	// 返回子组件列表的引用

protected:
	CComponent* ParentComponent = nullptr;
	std::vector<CComponent*> ChildComponents;
};

