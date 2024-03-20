#include "EngineMinimal.h"
#include "DynamicMap.h"

#include "Config/EngineRenderConfig.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/Core/RenderTarget.h"

FDynamicMap::FDynamicMap() = default;

void FDynamicMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	GeometryMap = inGeometryMap;
	DirectXPipelineState = inDirectXPipelineState;
	RenderLayers = inRenderLayer;
}

void FDynamicMap::SetBufferSize(int wid, int hei)
{
	Width = wid;
	Height = hei;
}

void FDynamicMap::PreDraw(float DeltaTime)
{
}

void FDynamicMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
}

void FDynamicMap::Build(const XMFLOAT3& center)
{
}

void FDynamicMap::Draw(float deltaTime)
{
}

void FDynamicMap::ResetView(int wid, int hei)
{
	Width = wid;
	Height = hei;
}

void FDynamicMap::CalculateSRVOffset()
{
	UINT CBVSRVUAVDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);	// 获取CBV/SRV/UAV描述符大小

	const auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();	// 获取SRV描述符句柄
	const auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();	// 获取SRV描述符句柄

	RenderTarget->GetCPUShaderResourceView() = CD3DX12_CPU_DESCRIPTOR_HANDLE(CPUSRVDesHeapStart, SRVOffset, CBVSRVUAVDescriptorSize);	// 设置SRV描述符句柄，将SRV描述符句柄指向SRV堆的指定偏移位置
	RenderTarget->GetGPUShaderResourceView() = CD3DX12_GPU_DESCRIPTOR_HANDLE(GPUSRVDesHeapStart, SRVOffset, CBVSRVUAVDescriptorSize);	// 设置SRV描述符句柄，将SRV描述符句柄指向SRV堆的指定偏移位置

}

void FDynamicMap::CalculateRTVOffset()
{
	UINT rtvDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);	// 获取RTV描述符大小

	D3D12_CPU_DESCRIPTOR_HANDLE rtvDesHandle = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();	// 获取RTV描述符句柄
	
	RenderTarget->GetCPURenderTargetView() = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvDesHandle, RTVOffset, rtvDescriptorSize);	// 设置RTV描述符句柄，将RTV描述符句柄指向RTV堆的指定偏移位置
	
}
