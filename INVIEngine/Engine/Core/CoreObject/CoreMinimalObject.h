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

protected:
	bool bTick;

};

extern std::vector<CCoreMinimalObject*> GObjects;		// 全局对象


