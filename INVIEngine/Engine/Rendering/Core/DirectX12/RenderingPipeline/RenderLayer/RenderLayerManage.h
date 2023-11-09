#pragma once
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayer;

class FRenderLayerManage
{
	friend class FRenderLayer;
	friend class FGeometry;

public:
	FRenderLayerManage();
	~FRenderLayerManage();

	// ��������ע��
	void Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState);

	virtual void PreDraw(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);

	// �㼶����
	void Sort();

	void BuildShader();

	static std::shared_ptr<FRenderLayer> FindByRenderLayer(int layer);

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


