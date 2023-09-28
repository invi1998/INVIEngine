#pragma once

#include "Core/CoreObject/CoreMinimalObject.h"


class CLightComponent;

class CLightManager : public CCoreMinimalObject
{
	friend struct FGeometryMap;

public:
	void AddLight(CLightComponent* lightInstance);

protected:
	std::vector<CLightComponent*> Lights;

};

