#pragma once

class CCoreMinimalObject;

template <typename T>
T* CreateObject(CCoreMinimalObject* owner, CCoreMinimalObject* NewObject)
{
	T* obj = dynamic_cast<T*>(NewObject);
	obj->Owner = owner;
	return obj;
}

template <typename T, typename... ParamType>
T* ConstructionObject(CCoreMinimalObject* owner, ParamType&& ... params)
{
	return CreateObject<T>(owner, new T(params...));
}


