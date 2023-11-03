#pragma once
// #include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"
class FRenderLayer;

class FRenderLayerManage
{
	friend class FRenderLayer;

public:
	FRenderLayerManage();
	~FRenderLayerManage();

	// ≤„º∂≈≈–Ú
	void Sort();

protected:
	static std::vector<std::shared_ptr<FRenderLayer>> RenderLayers;
};

template<typename T>
std::shared_ptr<T> CreateRenderLayer()
{
	std::shared_ptr<T> RenderLayer = std::make_shared<T>();

	RenderLayer->RegisterRenderLayer();

	return RenderLayer;
}


