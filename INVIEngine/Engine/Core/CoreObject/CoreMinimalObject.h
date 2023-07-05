#pragma once

class CCoreMinimalObject
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

	template<typename T>
	static T* CreateObject(CCoreMinimalObject* NewObject);
};

template <typename T>
T* CCoreMinimalObject::CreateObject(CCoreMinimalObject* NewObject)
{
	return dynamic_cast<T*>(NewObject);
}


