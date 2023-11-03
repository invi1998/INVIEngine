#pragma once

#include "Geometry/GeometryMap.h"
#include "Interface/DirectXDeviceInterface.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RenderLayer/RenderLayerManage.h"
#include "RootSignature/DirectXRootSignature.h"
#include "Shader/Core/Shader.h"

class CMeshComponent;
struct FViewportInfo;

// ��Ⱦ��ˮ��
class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(const size_t meshHash, CMeshComponent* Mesh, const FMeshRenderingData& MeshData);
	void BuildPipeline();
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info);

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);
	bool FindMeshRenderingDataByHash(size_t hashKey, FRenderingData& rendering_data);
	void DuplicateMesh(CMeshComponent* my_mesh, const FRenderingData& rendering_data);

protected:
	FGeometryMap GeometryMap;

	FRenderLayerManage RenderLayerManage;	// ��Ⱦ�㼶

	FDirectXPipelineState DirectXPipelineState;		// ����״̬ pso
	FDirectXRootSignature DirectXRootSignature;		// ��ǩ��
};

