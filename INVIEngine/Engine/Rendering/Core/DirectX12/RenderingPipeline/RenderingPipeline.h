#pragma once

#include "EngineMinimal.h"
#include "Geometry/GeometryMap.h"
#include "Interface/DirectXDeviceInterface.h"
#include "PipelineState/DirectXPipelineState.h"
#include "Shader/Core/Shader.h"

// ��Ⱦ��ˮ��
class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(CMesh* Mesh, const FMeshRenderingData& MeshData);
	void BuildPipeline();

protected:
	FGeometryMap GeometryMap;
	
	FShader VertexShader;	// ������ɫ��
	FShader PixelShader;	// ������ɫ��

	// �� Direct3D 12 �У����벼����һ�������������Ԫ�ص����ݽṹ����ʾ���㻺�����д洢�Ķ������ݵĸ�ʽ�����з�ʽ��
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		// �������벼�֣�Input Layout���еĵ���Ԫ�ء�

	FDirectXPipelineState DirectXPipelineState;
};

