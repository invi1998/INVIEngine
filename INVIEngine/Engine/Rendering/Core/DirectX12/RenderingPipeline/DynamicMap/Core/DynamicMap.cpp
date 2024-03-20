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
	UINT CBVSRVUAVDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);	// ��ȡCBV/SRV/UAV��������С

	const auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();	// ��ȡSRV���������
	const auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();	// ��ȡSRV���������

	RenderTarget->GetCPUShaderResourceView() = CD3DX12_CPU_DESCRIPTOR_HANDLE(CPUSRVDesHeapStart, SRVOffset, CBVSRVUAVDescriptorSize);	// ����SRV�������������SRV���������ָ��SRV�ѵ�ָ��ƫ��λ��
	RenderTarget->GetGPUShaderResourceView() = CD3DX12_GPU_DESCRIPTOR_HANDLE(GPUSRVDesHeapStart, SRVOffset, CBVSRVUAVDescriptorSize);	// ����SRV�������������SRV���������ָ��SRV�ѵ�ָ��ƫ��λ��

}

void FDynamicMap::CalculateRTVOffset()
{
	UINT rtvDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);	// ��ȡRTV��������С

	D3D12_CPU_DESCRIPTOR_HANDLE rtvDesHandle = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();	// ��ȡRTV���������
	
	RenderTarget->GetCPURenderTargetView() = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvDesHandle, RTVOffset, rtvDescriptorSize);	// ����RTV�������������RTV���������ָ��RTV�ѵ�ָ��ƫ��λ��
	
}
