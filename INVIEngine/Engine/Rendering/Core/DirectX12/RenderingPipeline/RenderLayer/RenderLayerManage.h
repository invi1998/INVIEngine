#pragma once
struct FViewportInfo;
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayer;

class FRenderLayerManage
{
	friend class FRenderLayer;
	friend class FGeometry;
	friend struct FGeometryMap;

public:
	FRenderLayerManage();
	~FRenderLayerManage();

	// 基础数据注册
	void Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState);

	virtual void PreDraw(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);

	// 层级排序
	void Sort();

	virtual void BuildPSO();

	static std::shared_ptr<FRenderLayer> FindByRenderLayer(int layer);

	virtual void UpdateCaculations(float deltaTime, const FViewportInfo& viewportInfo);

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


