#include "EngineMinimal.h"

#include "Component.h"

CComponent::~CComponent()
{
}

void CComponent::BeginInit()
{
	CCoreMinimalObject::BeginInit();
}

void CComponent::Tick(float DeltaTime)
{
	CCoreMinimalObject::Tick(DeltaTime);
}
