#pragma once
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayer;

class FRenderLayerManage
{
	friend class FRenderLayer;

public:
	FRenderLayerManage();
	~FRenderLayerManage();

	// ��������ע��
	void Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState);

	// �㼶����
	void Sort();

	void BuildShader();

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


