#pragma once
#include "Actor/Core/ActorObject.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineType.h"
struct FRenderingData;
class CMeshComponent;
struct FViewportInfo;
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayer;

class FRenderLayerManage : public IDirectXDeviceInterface
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

	void HighlightObject(GActorObject* object);
	void HighlightObject(std::weak_ptr<FRenderingData> weakRenderDate);
	void HighlightObject(CMeshComponent* commponent);

	static std::shared_ptr<FRenderLayer> FindByRenderLayer(int layer);

	virtual void UpdateCaculations(float deltaTime, const FViewportInfo& viewportInfo);

	// 指定你要渲染的层级
	void ResetPSO(int layer);
	void ResetPSO(int layer, EPipelineState ps);

	// 指定你要那个层级里面的模型进行渲染
	void DrawMesh(float DeltaTime, int layer, ERenderCondition rc = RC_Always);

	void Add(std::weak_ptr<FRenderingData>& weakRenderDate, int layer);

	void Remove(std::weak_ptr<FRenderingData>& weakRenderDate, int layer);		// 传入引用进行移除

	void Remove(const size_t hashKey, int layer);		// 传入hashKey进行移除

	void Clear(int layer);	// 清空层级内的所有渲染数据

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


