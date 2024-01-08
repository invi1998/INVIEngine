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

	// ��������ע��
	void Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState);

	virtual void PreDraw(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);

	// ͨ��ָ����Ⱦ��ID������ָ����Ⱦ�㼶
	virtual void Draw(int inLayer, float deltaTime);

	// ���ݴ����key��Ȼ����ָ���㼶�в���ģ�ͣ���Ը�ģ�ͽ�����Ⱦ
	virtual void FindObjectDraw(float DeltaTime, int layer, const CMeshComponent* key);

	// �㼶����
	void Sort();

	virtual void BuildPSO();

	void HighlightObject(GActorObject* object);
	void HighlightObject(std::weak_ptr<FRenderingData> weakRenderDate);
	void HighlightObject(CMeshComponent* commponent);

	static std::shared_ptr<FRenderLayer> FindByRenderLayer(int layer);

	virtual void UpdateCaculations(float deltaTime, const FViewportInfo& viewportInfo);

	// ָ����Ҫ��Ⱦ�Ĳ㼶
	void ResetPSO(int layer);
	void ResetPSO(int layer, EPipelineState ps);

	// ָ����Ҫ�Ǹ��㼶�����ģ�ͽ�����Ⱦ
	void DrawMesh(float DeltaTime, int layer, ERenderCondition rc = RC_Always);

	void Add(std::weak_ptr<FRenderingData>& weakRenderDate, int layer);

	void Remove(std::weak_ptr<FRenderingData>& weakRenderDate, int layer);		// �������ý����Ƴ�

	void Remove(const size_t hashKey, int layer);		// ����hashKey�����Ƴ�

	void Clear(int layer);	// ��ղ㼶�ڵ�������Ⱦ����

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


