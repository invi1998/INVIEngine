#pragma once

#include "Core/Engine.h"

class FRenderingInterface
{
	friend class FWindowsEngine;

public:
	FRenderingInterface();
	virtual ~FRenderingInterface();

	virtual void Draw(float DeltaTime);

public:
	bool operator==(const FRenderingInterface& InOther)
	{
		return guid_equal(&Guid, &InOther.Guid);
	}

private:
	static std::vector<FRenderingInterface*> RenderingInterface;

	simple_c_guid Guid;

};

