#pragma once
#include "EngineMinimal.h"
#include "Core/DirectXRenderingEngine.h"


class CDirectX12RenderingEngine : public CDirectXRenderingEngine
{
public:
	CDirectX12RenderingEngine();
	~CDirectX12RenderingEngine() override;
};

