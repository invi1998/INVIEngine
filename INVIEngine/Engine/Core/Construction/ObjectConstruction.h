#pragma once

class CCoreMinimalObject;

template <typename T>
T* CreateObject(CCoreMinimalObject* NewObject)
{
	return dynamic_cast<T*>(NewObject);
}

template <typename T, typename... ParamType>
T* ConstructionObject(ParamType&& ... params)
{
	return CreateObject<T>(new T(params...));
}


