#pragma once
#include "Core/CoreObject/CoreMinimalObject.h"

class CComponent : public CCoreMinimalObject
{
public:
	virtual ~CComponent() override;
	void BeginInit() override;
	void Tick(float DeltaTime) override;
};

