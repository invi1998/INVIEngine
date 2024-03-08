#pragma once

#include "GuidInterface.h"
#include "Core/Construction/ObjectConstruction.h"

class CCoreMinimalObject;

class CCoreMinimalObject : public IGuidInterface
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject() override;

	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

	bool IsTick() const { return bTick; }

	void SetOwner(CCoreMinimalObject* InOwner) { Owner = InOwner; }		// 设置所有者
	void SetName(std::string InName) { Name = InName; }					// 设置名字

	// 获取所有者
	FORCEINLINE CCoreMinimalObject* GetOwner() const { return Owner; }
	FORCEINLINE std::string GetName() const { return Name; }

protected:
	bool bTick;

	// owner 所有者, 一般是一个组件, 但是也有可能是一个对象
	CCoreMinimalObject* Owner;
	std::string Name;
};

extern std::vector<CCoreMinimalObject*> GObjects;		// 全局对象


