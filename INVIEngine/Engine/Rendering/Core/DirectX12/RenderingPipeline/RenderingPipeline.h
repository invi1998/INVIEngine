#pragma once

#include "DynamicMap/CubeMap/DynamicReflectionCubeMap.h"
#include "Geometry/GeometryMap.h"
#include "Interface/DirectXDeviceInterface.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RenderLayer/RenderLayerManage.h"
#include "RootSignature/DirectXRootSignature.h"

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
	bool FindMeshRenderingDataByHash(size_t hashKey, FRenderingData& rendering_data, int layer = -1);
	void DuplicateMesh(CMeshComponent* my_mesh, const FRenderingData& rendering_data);

protected:
	FGeometryMap GeometryMap;

	FRenderLayerManage RenderLayerManage;	// 渲染层级

	FDirectXPipelineState DirectXPipelineState;		// 管线状态 pso
	FDirectXRootSignature DirectXRootSignature;		// 根签名

	FDynamicReflectionCubeMap DynamicCubeMap;		// 动态CubeMap
};

