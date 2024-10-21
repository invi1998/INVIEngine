#pragma once
#include "ConstructionComponent.h"

class CCoreMinimalObject;
class CComponent;

struct FCreateObjectParams
{
	FCreateObjectParams(CCoreMinimalObject* InOwner, CComponent* InParentComponent)
		: Owner(InOwner)
		, ParentComponent(InParentComponent)
	{}

	FCreateObjectParams() = default;

	CCoreMinimalObject* Owner = nullptr;
	CComponent* ParentComponent = nullptr;
};

template <typename T>
T* CreateObject(const FCreateObjectParams& objectParam, CCoreMinimalObject* NewObject)
{
	ConstructionComponent::ConstructionComponent(objectParam.Owner, NewObject);

	T* obj = dynamic_cast<T*>(NewObject);
	obj->SetOwner(objectParam.Owner);
	return obj;
}

template <typename T, typename... ParamType>
T* ConstructionObject(const FCreateObjectParams& objectParam, ParamType&& ... params)
{
	return CreateObject<T>(objectParam, new T(params...));
}


