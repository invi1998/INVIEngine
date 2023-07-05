#pragma once

#include "GuidInterface.h"

class CCoreMinimalObject : public IGuidInterface
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject() override;

	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

	template<typename T>
	static T* CreateObject(CCoreMinimalObject* NewObject);

	bool IsTick() const { return bTick; }

protected:
	bool bTick;

};

extern std::vector<CCoreMinimalObject*> GObjects;		// 全局对象

template <typename T>
T* CCoreMinimalObject::CreateObject(CCoreMinimalObject* NewObject)
{
	return dynamic_cast<T*>(NewObject);
}


