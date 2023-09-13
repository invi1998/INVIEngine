#include "LightManager.h"

void CLightManager::AddLight(CLightComponent* lightInstance)
{
	Lights.push_back(lightInstance);
}
