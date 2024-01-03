#pragma once

#include "GuidInterface.h"
#include "Core/Construction/ObjectConstruction.h"

class CCoreMinimalObject : public IGuidInterface
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject() override;

	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

	bool IsTick() const { return bTick; }

	// 获取所有者
	FORCEINLINE CCoreMinimalObject* GetOwner() const { return Owner; }

protected:
	bool bTick;

	// owner 所有者, 一般是一个组件, 但是也有可能是一个对象
	CCoreMinimalObject* Owner;
};

extern std::vector<CCoreMinimalObject*> GObjects;		// 全局对象


