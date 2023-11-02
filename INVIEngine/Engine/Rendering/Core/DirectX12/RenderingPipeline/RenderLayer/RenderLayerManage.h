#pragma once
class FRenderLayer;

class FRenderLayerManage
{
public:
	FRenderLayerManage();

protected:
	std::vector<std::shared_ptr<FRenderLayer>> RenderLayers;
};

