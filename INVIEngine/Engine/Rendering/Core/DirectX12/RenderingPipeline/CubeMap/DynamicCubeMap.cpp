#include "EngineMinimal.h"
#include "DynamicCubeMap.h"

#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/CubeMapRenderTarget.h"


FDynamicCubeMap::FDynamicCubeMap()
{
}

void FDynamicCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	GeometryMap = inGeometryMap;
	DirectXPipelineState = inDirectXPipelineState;
	RenderLayers = inRenderLayer;
}

void FDynamicCubeMap::Draw(float DeltaTime)
{
	// ָ���ĸ���Դ��ת����״̬����Ϊ����������buffer�������ڲ��Ͻ���
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		CubeMapRenderTarget->GetRenderTarget(),				// ��ǰbuffer������
		D3D12_RESOURCE_STATE_GENERIC_READ,		// ��ǰ״̬ ��ʾ��Դ��������������Ŀ��
		D3D12_RESOURCE_STATE_RENDER_TARGET	// Ŀ��״̬ �⽫ʹ��Դ����������ȾĿ�꣬���������Ը���Դִ�г��ֲ�����д��״̬��
	);
	GetD3dGraphicsCommandList()->ResourceBarrier(
		1,
		&ResourceBarrierPresent
	);

	D3D12_VIEWPORT Viewport = CubeMapRenderTarget->GetViewport();
	D3D12_RECT Rect = CubeMapRenderTarget->GetScissorRect();

	// ���ã����£��ӿ���Ϣ���ü�������Ϣ
	GetD3dGraphicsCommandList()->RSSetViewports(1, &Viewport);
	GetD3dGraphicsCommandList()->RSSetScissorRects(1, &Rect);

	for (size_t i = 0; i < 6; i++)
	{
		// �������
		constexpr float ColorBG[] = { 0.1f, 0.105f, 0.11f, 1.0f };
		GetD3dGraphicsCommandList()->ClearRenderTargetView(
			CubeMapRenderTarget->CPURenderTargetView[i],		// Ҫ�������ȾĿ����ͼ
			ColorBG,		// ������ɫ
			0,		// ���������������Ǻ��ӿ���صģ����ﲻ���������ã��������ר�ŵ����÷���
			nullptr
		);

		// �����Ⱥ�ģ�建����
		GetD3dGraphicsCommandList()->ClearDepthStencilView(
			DSVCubeMapCPUDesc,	// ����CPU�ڴ棨Ҫ��������ģ�建�����ڴ棩
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,		// �����Ⱥ�ģ�建����
			1.f,	// ��1��������ǵ���Ȼ�����������Ȼ���������Ϊ1
			0,		// ��0��������ǵ�ģ�建������ģ�建��������Ϊ0
			0,
			nullptr
		);

		// ָ����Ⱦ������������ϲ��׶Σ�������ȾĿ����ͼ
		GetD3dGraphicsCommandList()->OMSetRenderTargets(
			1,									// ָ����ȾĿ���� 1
			&CubeMapRenderTarget->CPURenderTargetView[i],		// ָ����ȾĿ��
			true,								// true�������Ǵ���ľ����һ���ڴ�������������ָ��
			&DSVCubeMapCPUDesc		// �������
		);

		// ��Ⱦ

		// ��Ⱦ�ƹ⣬���ʣ���ͼ
		GeometryMap->Draw(DeltaTime);
		// ��Ⱦ�㼶������ģ��
		RenderLayers->Draw(DeltaTime);
	}

	

	// ���õ�ǰ������buffer״̬
	CD3DX12_RESOURCE_BARRIER ResourceBarrierRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		CubeMapRenderTarget->GetRenderTarget(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_GENERIC_READ
	);
	GetD3dGraphicsCommandList()->ResourceBarrier(
		1,
		&ResourceBarrierRenderTarget
	);

}

void FDynamicCubeMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	if (CubeMapViewPorts.size() == 6)
	{
		// �����ӿ�
		for (size_t i = 0; i < 6; i++)
		{
			FViewportInfo tempViewport{};
			XMFLOAT3 position = CubeMapViewPorts[i]->GetPosition();
			tempViewport.CameraPosition = XMFLOAT4{ position.x, position.y, position.z, 1.f};
			tempViewport.ViewMatrix = CubeMapViewPorts[i]->GetViewMatrix();
			tempViewport.ProjectionMatrix = CubeMapViewPorts[i]->GetProjectionMatrix();

			GeometryMap->UpdateCalculationViewport(tempViewport, i+1);
		}
	}
	
}

void FDynamicCubeMap::BuildViewPort(const XMFLOAT3& InCenterPoint)
{
	struct FTempViewportCapture
	{
		XMFLOAT3 TargetPoint[6];
		XMFLOAT3 Up[6];
	};
	// +x, -x, +y, -y, +z, -z
	FTempViewportCapture Capture{};

	// �����������6����
	Capture.TargetPoint[0] = XMFLOAT3{ InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z};
	Capture.TargetPoint[1] = XMFLOAT3{ InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z};
	Capture.TargetPoint[2] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z};
	Capture.TargetPoint[3] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z};
	Capture.TargetPoint[4] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f};
	Capture.TargetPoint[5] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f };

	
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 0.f, -1.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 0.f, 1.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f };

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewPorts.push_back(CreateObject<GClientViewPort>(new GClientViewPort()));
		GClientViewPort* viewport = CubeMapViewPorts[CubeMapViewPorts.size() - 1];

		viewport->FaceTarget(InCenterPoint, Capture.TargetPoint[i], Capture.Up[i]);
		viewport->SetFrustum(XM_PIDIV2, 1, 1, 0.1f, 10000.f);
		viewport->BuildViewMatrix();
	}
}

void FDynamicCubeMap::BuildDepthStencil()
{
}
