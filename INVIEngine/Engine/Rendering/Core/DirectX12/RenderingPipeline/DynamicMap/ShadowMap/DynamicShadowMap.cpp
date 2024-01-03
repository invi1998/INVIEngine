#include "EngineMinimal.h"
#include "DynamicShadowMap.h"

#include "Component/Light/Core/LightComponent.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Manage/LightManager.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineType.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/ShadowMapRenderTarget.h"

FDynamicShadowMap::FDynamicShadowMap()
{
	CreateRenderTarget<FShadowMapRenderTarget>();
}

FDynamicShadowMap::~FDynamicShadowMap()
{
}

void FDynamicShadowMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FDynamicMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FDynamicShadowMap::PreDraw(float DeltaTime)
{
	FDynamicMap::PreDraw(DeltaTime);
}

void FDynamicShadowMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FDynamicMap::UpdateCalculations(delta_time, viewport_info);

	if (ShadowViewPort)
	{
		for (size_t i = 0; i < GetLightManger()->GetLights().size(); i++)
		{
			if (CLightComponent* lightComponent = GetLightManger()->GetLights()[i])
			{
				FViewportInfo ShadowViewInfo{};
				ShadowViewInfo.ViewMatrix = GetViewMatrix();
				ShadowViewInfo.ProjectionMatrix = GetProjectionMatrix();
				XMFLOAT3 pos = ShadowViewPort->GetPosition();
				ShadowViewInfo.CameraPosition = XMFLOAT4(pos.x, pos.y, pos.z, 1.0f);
				// �����ӿڣ�ƫ����Ϊ��̬������������+���ӿڵ������
				GeometryMap->UpdateCalculationViewport(ShadowViewInfo, GeometryMap->GetDynamicReflectionViewportNum() + 1);
			}
		}
	}
}

void FDynamicShadowMap::Build(const XMFLOAT3& center)
{
	FDynamicMap::Build(center);
}

void FDynamicShadowMap::Draw(float deltaTime)
{
	FDynamicMap::Draw(deltaTime);


	for (size_t i = 0; i < GetLightManger()->GetLights().size(); i++)
	{
		if (CLightComponent* lightComponent = GetLightManger()->GetLights()[i])
		{
			if (FShadowMapRenderTarget* innerRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(RenderTarget.get()))
			{
				if (lightComponent->GetLightType() == PointLight)
				{
					continue;
				}

				// ������Դ״̬Ϊ��д
				CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
					innerRenderTarget->GetRenderTarget(),				// ��Դ
					D3D12_RESOURCE_STATE_GENERIC_READ,				// ��Դ״̬ �ɶ�
					D3D12_RESOURCE_STATE_DEPTH_WRITE);		// ��Դ״̬ תΪ ��д

				GetD3dGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier);

				// ��Ҫÿ֡���µ����� �󶨾��ο�
				auto RenderTargetViewPort = innerRenderTarget->GetViewport();
				auto RenderTargetScissorRect = innerRenderTarget->GetScissorRect();

				GetD3dGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewPort);
				GetD3dGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);

				UINT CBVDescriptorSize = GeometryMap->ViewportConstantBufferViews.GetConstantBufferByteSize();

				// ������ģ�建����
				GetD3dGraphicsCommandList()->ClearDepthStencilView(
					innerRenderTarget->CPUDepthStencilView,		// DSV ������
					D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,					// ������ģ�建����
					1.0f,											// ���ֵ
					0,												// ģ��ֵ
					0,												// �����������
					nullptr);										// �������)

				// ����ϲ���Ӱ��ͼ ����Ϊ���ﲻ���ǽ���������ɫ�����Բ���Ҫ������ȾĿ�꣨��ȾĿ������Ϊ0����
				GetD3dGraphicsCommandList()->OMSetRenderTargets(
					0,				// ��ȾĿ������
					nullptr,		// ƫ��
					false,			// �Ƿ���õ������������ȾĿ����ͼ�����ģ����ͼ
					&innerRenderTarget->CPUDepthStencilView);		// ָ�����ģ����ͼ

				// �������
				auto ViewportAddr = GeometryMap->ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();
				ViewportAddr += (1 + GeometryMap->GetDynamicReflectionViewportNum()) * CBVDescriptorSize;

				GetD3dGraphicsCommandList()->SetGraphicsRootShaderResourceView(
					1,	// ����������ʼ����
					ViewportAddr	// GPU��Դ��ͼ
				);

				DrawShadowMapTexture(deltaTime);

				UINT psoType = EPipelineState::OrthographicShadow;
				switch (lightComponent->GetLightType())
				{
					case DirectionalLight:
					{
						psoType = EPipelineState::OrthographicShadow;
						break;
					};
					case PointLight: break;
					case SpotLight:
					{
						psoType = EPipelineState::PerspectiveShadow;
						break;
					};
					default: break;
				}
				RenderLayers->ResetPSO(RENDER_LAYER_OPAQUE_SHADOW, static_cast<EPipelineState>(psoType));

				RenderLayers->DrawMesh(deltaTime, RENDER_LAYER_OPAQUE, ERenderCondition::RC_Shadow);
				RenderLayers->DrawMesh(deltaTime, RENDER_LAYER_TRANSPARENT, ERenderCondition::RC_Shadow);
				RenderLayers->DrawMesh(deltaTime, RENDER_LAYER_OPAQUE_REFLECT, ERenderCondition::RC_Shadow);

				// ����Դ״̬�ɿ�дת��Ϊ�ɶ�
				CD3DX12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
					innerRenderTarget->GetRenderTarget(),				// ��Դ
					D3D12_RESOURCE_STATE_DEPTH_WRITE,				// ��Դ״̬ ��д
					D3D12_RESOURCE_STATE_GENERIC_READ);		// ��Դ״̬ תΪ �ɶ�

				GetD3dGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier2);
			}
		}
	}
}

void FDynamicShadowMap::SetViewportPosition(const XMFLOAT3& position)
{
	ShadowViewPort->SetPosition(position);
	BuildViewMatrix();
}

void FDynamicShadowMap::SetViewportRotation(const XMFLOAT3& rotation)
{
	ShadowViewPort->SetRotation(fvector_3d(rotation.x, rotation.y, rotation.z));
	BuildViewMatrix();
}

void FDynamicShadowMap::BuildViewPort(const XMFLOAT3& position)
{

	ShadowViewPort = CreateObject<GClientViewPort>(FCreateObjectParams(), new GClientViewPort());
	ShadowViewPort->SetPosition(position);
	ShadowViewPort->FaceTarget(position, { 10.f, 0.f, 0.f });
	ShadowViewPort->SetFrustum(XM_PIDIV2, 1.f, 1.f, 0.1f, 10000.f);
	BuildViewMatrix();
}

XMFLOAT4X4 FDynamicShadowMap::GetViewMatrix()
{
	return ShadowViewPort->GetViewMatrix();
}

XMFLOAT4X4 FDynamicShadowMap::GetProjectionMatrix()
{
	return ShadowViewPort->GetProjectionMatrix();
}

void FDynamicShadowMap::BuildViewMatrix()
{
	ShadowViewPort->BuildViewMatrix();
}

void FDynamicShadowMap::BuildOrthographicOffCenterProjectionMatrixLH(float radius, const XMFLOAT3& centerPos)
{
	ShadowViewPort->BuildOrthographicOffCenterProjectionMatrixLH(radius, centerPos);
}

void FDynamicShadowMap::BuildPerspectiveProjectionMatrixLH(float radius, const XMFLOAT3& centerPos)
{
	ShadowViewPort->SetFrustum(0.9f * XM_PI, 1.0f, 1.0f, 0.1f, radius);
}

void FDynamicShadowMap::BuildProjectionMatrix()
{
}

void FDynamicShadowMap::BuildOrthoProjectionMatrix(const XMFLOAT3& targetPosition, const XMFLOAT3& direction, float radius)
{
	XMVECTOR targetPositionVEC = XMLoadFloat3(&targetPosition);
	XMVECTOR directionVEC = XMLoadFloat3(&direction);

	// �õ��ƹ�λ��
	XMVECTOR viewPositionVEC = directionVEC * -radius;
	ShadowViewPort->SetPosition(viewPositionVEC);

	XMFLOAT3 viewPosition{};
	XMStoreFloat3(&viewPosition, viewPositionVEC);

	ShadowViewPort->FaceTarget(viewPosition, targetPosition, { 0.f, 1.f, 0.f });

	BuildViewMatrix();

	BuildOrthographicOffCenterProjectionMatrixLH(radius, targetPosition);
}

void FDynamicShadowMap::BuildPerspectiveProjectionMatrix(const XMFLOAT3& position, const XMFLOAT3& direction, float radius)
{
	XMVECTOR positionVEC = XMLoadFloat3(&position);
	XMVECTOR directionVEC = XMLoadFloat3(&direction);

	// �õ��ƹ�λ��
	ShadowViewPort->SetPosition(positionVEC);

	// �õ�Ŀ��λ��
	XMVECTOR viewTargetPositionVEC = directionVEC * -radius;
	XMFLOAT3 viewTargetPosition{};
	XMStoreFloat3(&viewTargetPosition, viewTargetPositionVEC);

	ShadowViewPort->FaceTarget(position, viewTargetPosition, { 0.f, 1.f, 0.f });

	BuildViewMatrix();

	BuildProjectionMatrix();

	BuildPerspectiveProjectionMatrixLH(radius, viewTargetPosition);
}

void FDynamicShadowMap::BuildDepthStencilViewDesc()
{
	// �õ�DSV��������ƫ��ֵ
	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	if (FShadowMapRenderTarget* innerRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(RenderTarget.get()))
	{
		innerRenderTarget->CPUDepthStencilView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),		// ���ӿ�DSV���ڴ˻�����ƫ��
			1 + 1,													// 1�����ӿ�DSV + 1��CubeMap��DSV��ƫ�ƹ�������֮�󣬲�����Ӱ��DSV
			DescriptorHandleIncrementSize);							// ������ƫ��ֵ
	}
}

void FDynamicShadowMap::BuildShadowMapRenderTargetDescriptor()
{
	BuildRenderTargetSRV();

	RenderTarget->Init(Width, Height, DXGI_FORMAT_D24_UNORM_S8_UINT);
}

void FDynamicShadowMap::DrawShadowMapTexture(float DeltaTime)
{
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
			7,	// ����������ʼ����
			RenderTarget->GetGPUShaderResourceView()	// GPU��Դ��ͼ
		);
}

void FDynamicShadowMap::BuildRenderTargetSRV()
{
	if (FShadowMapRenderTarget* inRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(this->RenderTarget.get()))
	{
		UINT CBVDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

		int ShadowMapOffset = GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount() + 1;

		// ΪCubeMap����CPU shader��Դ��ͼ ��Ҫ�Ǵ���Shadow����������
		inRenderTarget->CPUShaderResourceView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			CPUSRVDesHeapStart,		// CPU SRV����ʼ��ַ
			ShadowMapOffset,	// ƫ����
			CBVDescriptorSize	// SRVƫ����
		);

		// ΪCubeMap����GPU shader��Դ��ͼ ����������Ⱦ��Ӱ��ͼ��ʱ����Ҫ�������Դ��ͼ�󶨵���ɫ����
		inRenderTarget->GPUShaderResourceView = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			GPUSRVDesHeapStart,		// GPU SRV����ʼ��ַ
			ShadowMapOffset,	// ƫ����
			CBVDescriptorSize	// SRVƫ����
		);
	}
}
