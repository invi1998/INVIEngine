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

protected:
	CComponent* ParentComponent = nullptr;
	std::vector<CComponent*> ChildComponents{};
};

