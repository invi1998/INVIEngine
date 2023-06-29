#pragma once

#include "Core/Engine.h"

class IRenderingInterface
{
	friend class FWindowsEngine;

public:
	IRenderingInterface();
	virtual ~IRenderingInterface();

	virtual void Init();
	virtual void Draw(float DeltaTime);

public:
	bool operator==(const IRenderingInterface& InOther)
	{
		return guid_equal(&Guid, &InOther.Guid);
	}

private:
	static std::vector<IRenderingInterface*> RenderingInterface;

	simple_c_guid Guid;

};

