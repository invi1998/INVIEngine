#include "Rendering.h"

std::vector<FRenderingInterface*> FRenderingInterface::RenderingInterface;

FRenderingInterface::FRenderingInterface()
{
	create_guid(&Guid);

	RenderingInterface.push_back(this);
}

FRenderingInterface::~FRenderingInterface()
{
	for (std::vector<FRenderingInterface*>::const_iterator iter = RenderingInterface.begin(); iter != RenderingInterface.end(); ++iter)
	{
		if (*iter == this)
		{
			RenderingInterface.erase(iter);
			break;
		}
	}
}

void FRenderingInterface::Draw(float DeltaTime)
{
}
