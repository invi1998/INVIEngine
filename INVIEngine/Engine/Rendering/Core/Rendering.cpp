#include "EngineMinimal.h"

#include "Rendering.h"

#include "Platform/Windows/WindowsEngine.h"
#include "Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"

IRenderingInterface::IRenderingInterface()
{
	// RenderingInterface.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	/*for (std::vector<IRenderingInterface*>::const_iterator iter = RenderingInterface.begin(); iter != RenderingInterface.end(); ++iter)
	{
		if (*iter == this)
		{
			RenderingInterface.erase(iter);
			break;
		}
	}*/
}

void IRenderingInterface::Init()
{
}

void IRenderingInterface::PreDraw(float DeltaTime)
{
	// 重置命令列表，因为我们每一帧都会有新的提交列表
	// ANALYSIS_RESULT(GetD3dGraphicsCommandList()->Reset(GetCommandAllocator().Get(), nullptr));
}

void IRenderingInterface::PostDraw(float DeltaTime)
{
}

void IRenderingInterface::Draw(float DeltaTime)
{
}

