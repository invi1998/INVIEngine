#include "EngineMinimal.h"
#include "RenderLayer.h"

#include "Component/Mesh/Core/MeshComponent.h"
#include "Material/Core/Material.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"

FRenderLayer::FRenderLayer(): RenderPriority(0)
{
}

void FRenderLayer::RegisterRenderLayer()
{
	FRenderLayerManage::RenderLayers.push_back(this->shared_from_this());
}

void FRenderLayer::PreDraw(float deltaTime)
{
}

void FRenderLayer::Draw(float deltaTime)
{
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (auto& innerRenderData : RenderData)
	{
		// 获取顶点/索引缓冲区视图
		D3D12_VERTEX_BUFFER_VIEW vbv = GeometryMap->Geometries[innerRenderData.GeometryKey].GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW ibv = GeometryMap->Geometries[innerRenderData.GeometryKey].GetIndexBufferView();


		CD3DX12_GPU_DESCRIPTOR_HANDLE DesMeshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart());	// 模型描述handle

		// 设置索引视图
		GetD3dGraphicsCommandList()->IASetIndexBuffer(&ibv);

		// 绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
		GetD3dGraphicsCommandList()->IASetVertexBuffers(
			0,		// 指定起始的顶点缓冲区槽号 `StartSlot`，这个值通常为 0 值范围 0-15
			1,		// 指定要绑定的顶点缓冲区数量 NumViews，这个值必须小于或等于硬件支持的最大顶点缓冲区数量
			&vbv	// 使用顶点缓冲区视图数组 pViews 来描述每个顶点缓冲区的位置、大小和布局等信息。
		);

		// 指定图元类型（点，线，面） 下面设置为 绘制由三角形构成的列表
		const D3D_PRIMITIVE_TOPOLOGY DisplayType = static_cast<D3D_PRIMITIVE_TOPOLOGY>((*innerRenderData.Mesh->GetMaterial())[0]->GetMaterialDisplayType());
		GetD3dGraphicsCommandList()->IASetPrimitiveTopology(DisplayType);

		// 模型起始偏移
		DesMeshHandle.Offset(innerRenderData.MeshObjectIndex, DescriptorOffset);
		GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);

		// 上述步骤只是在提交数据到GPU，并不是在渲染模型

		// 绘制图元（真实渲染模型）
		GetD3dGraphicsCommandList()->DrawIndexedInstanced(
			innerRenderData.IndexSize,						// 顶点数量
			1,												// 表示要绘制的实例数。如果只需要绘制单个实例，则可以将此参数设置为 1。
			innerRenderData.IndexOffsetPosition,				// 表示从索引缓冲区中哪个位置开始读取索引数据。通常情况下，这个值为 0。
			innerRenderData.VertexOffsetPosition,				// 表示每个顶点在顶点缓冲区中相对于起始位置的偏移量。如果顶点数据是连续排列的，则可以将此参数设置为 0。
			0												// 表示从第几个实例开始绘制。如果只需要绘制单个实例，则可以将此参数设置为 0。
		);
	}
}

void FRenderLayer::PostDraw(float deltaTime)
{
}

void FRenderLayer::Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState)
{
	GeometryMap = geometry;
	DirectXPipelineState = directXPipelineState;
}

