#pragma once
class CMeshComponent;
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

	// 通过指定渲染层ID来定向指定渲染层级
	virtual void Draw(int inLayer, float deltaTime);

	// 根据传入的key，然后在指定层级中查找模型，针对该模型进行渲染
	virtual void FindObjectDraw(float DeltaTime, int layer, const CMeshComponent* key);

	// 层级排序
	void Sort();

	virtual void BuildPSO();

	static std::shared_ptr<FRenderLayer> FindByRenderLayer(int layer);

	virtual void UpdateCaculations(float deltaTime, const FViewportInfo& viewportInfo);

	// 指定你要渲染的层级
	void ResetPSO(int layer);

	// 指定你要那个层级里面的模型进行渲染
	void DrawMesh(float DeltaTime, int layer);

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


