#pragma once
#include "Interface/DirectXDeviceInterface.h"
#include "Shader/Core/Shader.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/RenderingData.h"

struct FDirectXPipelineState;
struct FGeometryMap;

class FRenderLayer : public IDirectXDeviceInterface, public std::enable_shared_from_this<FRenderLayer>
{
	friend struct FGeometry;
public:
	FRenderLayer();
	virtual ~FRenderLayer() = default;

	void RegisterRenderLayer();

	const UINT GetRenderPriority() const { return RenderPriority; }


public:
	virtual void Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState);

	virtual void BuildShader() = 0;

	virtual int GetRenderLayerType() const = 0;

protected:
	UINT RenderPriority;

	FShader VertexShader;
	FShader PixelShader;

	// �� Direct3D 12 �У����벼����һ�������������Ԫ�ص����ݽṹ����ʾ���㻺�����д洢�Ķ������ݵĸ�ʽ�����з�ʽ��
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		// �������벼�֣�Input Layout���еĵ���Ԫ�ء�

	std::vector<FRenderingData> RenderData;

	FGeometryMap* GeometryMap;
	FDirectXPipelineState* DirectXPipelineState;	// ����״̬ pso
};

