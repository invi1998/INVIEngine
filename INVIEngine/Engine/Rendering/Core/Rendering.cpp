#include "Rendering.h"

std::vector<IRenderingInterface*> IRenderingInterface::RenderingInterface;

IRenderingInterface::IRenderingInterface()
{
	create_guid(&Guid);

	RenderingInterface.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	for (std::vector<IRenderingInterface*>::const_iterator iter = RenderingInterface.begin(); iter != RenderingInterface.end(); ++iter)
	{
		if (*iter == this)
		{
			RenderingInterface.erase(iter);
			break;
		}
	}
}

void IRenderingInterface::Init()
{
}

void IRenderingInterface::Draw(float DeltaTime)
{
}

ComPtr<ID3D12Device> IRenderingInterface::GetD3dDevice()
{
	if (Engine)
	{
		return Engine->D3dDevice;
	}

	return nullptr;
}

ComPtr<ID3D12GraphicsCommandList> IRenderingInterface::GetD3dGraphicsCommandList()
{
	if (Engine)
	{
		return Engine->GraphicsCommandList;
	}

	return nullptr;
}

