#pragma once

#include "Geometry/GeometryMap.h"
#include "Interface/DirectXDeviceInterface.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RenderLayer/RenderLayerManage.h"
#include "RootSignature/DirectXRootSignature.h"
#include "Shader/Core/Shader.h"

class CMeshComponent;
struct FViewportInfo;

// 渲染流水线
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

	FRenderLayerManage RenderLayerManage;	// 渲染层级

	FDirectXPipelineState DirectXPipelineState;		// 管线状态 pso
	FDirectXRootSignature DirectXRootSignature;		// 根签名
};

