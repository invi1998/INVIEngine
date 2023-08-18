#pragma once

#include "EngineMinimal.h"
#include "Geometry/GeometryMap.h"
#include "Interface/DirectXDeviceInterface.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RootSignature/DirectXRootSignature.h"
#include "Shader/Core/Shader.h"

struct FViewportInfo;

// ��Ⱦ��ˮ��
class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(CMesh* Mesh, const FMeshRenderingData& MeshData);
	void BuildPipeline();
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info);

protected:
	FGeometryMap GeometryMap;
	
	FShader VertexShader;	// ������ɫ��
	FShader PixelShader;	// ������ɫ��

	// �� Direct3D 12 �У����벼����һ�������������Ԫ�ص����ݽṹ����ʾ���㻺�����д洢�Ķ������ݵĸ�ʽ�����з�ʽ��
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		// �������벼�֣�Input Layout���еĵ���Ԫ�ء�

	FDirectXPipelineState DirectXPipelineState;		// ����״̬ pso
	FDirectXRootSignature DirectXRootSignature;		// ��ǩ��
};

