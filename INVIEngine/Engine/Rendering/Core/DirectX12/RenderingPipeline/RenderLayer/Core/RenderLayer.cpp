#include "EngineMinimal.h"
#include "RenderLayer.h"

#include "Component/Mesh/Core/MeshComponent.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Material/Core/Material.h"
#include "Mesh/Core/ObjectTransformation.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"
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
	for (auto& innerRenderData : RenderData)
	{
		DrawObject(deltaTime, innerRenderData);
	}
}

void FRenderLayer::PostDraw(float deltaTime)
{
}

void FRenderLayer::DrawObject(float deltaTime, const FRenderingData& renderDate)
{
	UINT MeshOffset = GeometryMap->MeshConstantBufferViews.GetConstantBufferByteSize();

	// ��ȡ����/������������ͼ
	D3D12_VERTEX_BUFFER_VIEW vbv = GeometryMap->Geometries[renderDate.GeometryKey].GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW ibv = GeometryMap->Geometries[renderDate.GeometryKey].GetIndexBufferView();


	// CD3DX12_GPU_DESCRIPTOR_HANDLE DesMeshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart());	// ģ������handle

	D3D12_GPU_VIRTUAL_ADDRESS VirtualMeshAddress = GeometryMap->MeshConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();

	// ����������ͼ
	GetD3dGraphicsCommandList()->IASetIndexBuffer(&ibv);

	// ����Ⱦ��ˮ���ϵ�����ۣ�����������װ�����׶δ��붥������
	GetD3dGraphicsCommandList()->IASetVertexBuffers(
		0,		// ָ����ʼ�Ķ��㻺�����ۺ� `StartSlot`�����ֵͨ��Ϊ 0 ֵ��Χ 0-15
		1,		// ָ��Ҫ�󶨵Ķ��㻺�������� NumViews�����ֵ����С�ڻ����Ӳ��֧�ֵ���󶥵㻺��������
		&vbv	// ʹ�ö��㻺������ͼ���� pViews ������ÿ�����㻺������λ�á���С�Ͳ��ֵ���Ϣ��
	);

	// ָ��ͼԪ���ͣ��㣬�ߣ��棩 ��������Ϊ �����������ι��ɵ��б�
	const D3D_PRIMITIVE_TOPOLOGY DisplayType = static_cast<D3D_PRIMITIVE_TOPOLOGY>((*renderDate.Mesh->GetMaterial())[0]->GetMaterialDisplayType());
	GetD3dGraphicsCommandList()->IASetPrimitiveTopology(DisplayType);

	// ģ����ʼƫ��
	// DesMeshHandle.Offset(innerRenderData.MeshObjectIndex, DescriptorOffset);
	// GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);

	// ÿ���������Geometry�׵�ַ��ƫ��
	D3D12_GPU_VIRTUAL_ADDRESS VAddress = VirtualMeshAddress + renderDate.MeshObjectIndex * MeshOffset;
	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(0, VAddress);

	// ��������ֻ�����ύ���ݵ�GPU������������Ⱦģ��

	// ����ͼԪ����ʵ��Ⱦģ�ͣ�
	GetD3dGraphicsCommandList()->DrawIndexedInstanced(
		renderDate.IndexSize,						// ��������
		1,												// ��ʾҪ���Ƶ�ʵ���������ֻ��Ҫ���Ƶ���ʵ��������Խ��˲�������Ϊ 1��
		renderDate.IndexOffsetPosition,				// ��ʾ���������������ĸ�λ�ÿ�ʼ��ȡ�������ݡ�ͨ������£����ֵΪ 0��
		renderDate.VertexOffsetPosition,				// ��ʾÿ�������ڶ��㻺�������������ʼλ�õ�ƫ��������������������������еģ�����Խ��˲�������Ϊ 0��
		0												// ��ʾ�ӵڼ���ʵ����ʼ���ơ����ֻ��Ҫ���Ƶ���ʵ��������Խ��˲�������Ϊ 0��
	);
}

void FRenderLayer::FindObjectDraw(float delta_time, const CMeshComponent* key)
{
	for (const auto& renderData : RenderData)
	{
		if (renderData.Mesh == key)
		{
			DrawObject(delta_time, renderData);
			break;
		}
	}
}

void FRenderLayer::BuildShaderMacro(std::vector<ShaderType::FShaderMacro>& inShaderMacro)
{
	//inShaderMacro = std::initializer_list{
	//	{"TEXTURE2DNUM", _itoa(GeometryMap->GetDrawTexture2DCount(), TextureNumBuff, 10)},
	//	{"CUBE_MAP_NUM", _itoa(GeometryMap->GetDrawCubeMapCount(), TextureNumBuff, 10)},
	//	{"START_UP_FOG", _itoa(GeometryMap->IsStartUpFog(), TextureNumBuff, 10)},
	//	// NULL, NULL,
	//};

	{
		ShaderType::FShaderMacro ShaderMacro;

		char TextureNumBuff[10] = { 0 };
		ShaderMacro.Name = "TEXTURE2D_MAP_NUM";
		ShaderMacro.Definition = _itoa(GeometryMap->GetDrawTexture2DCount(), TextureNumBuff, 10);

		inShaderMacro.push_back(ShaderMacro);
	}

	{
		ShaderType::FShaderMacro ShaderMacro;

		char TextureNumBuff[10] = { 0 };
		ShaderMacro.Name = "CUBE_MAP_NUM";
		ShaderMacro.Definition = _itoa(GeometryMap->GetDrawCubeMapCount(), TextureNumBuff, 10);

		inShaderMacro.push_back(ShaderMacro);
	}

	{
		ShaderType::FShaderMacro ShaderMacro;
		ShaderMacro.Name = "START_UP_FOG";
		ShaderMacro.Definition = GeometryMap->IsStartUpFog() ? "1" : "0";

		inShaderMacro.push_back(ShaderMacro);
	}
}


void FRenderLayer::Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState)
{
	GeometryMap = geometry;
	DirectXPipelineState = directXPipelineState;
}

void FRenderLayer::UpdateCaculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (auto& renderingData : RenderData)
	{
		// ����ģ��
		XMFLOAT3& Position = renderingData.Mesh->GetPosition();
		XMFLOAT3& Scale = renderingData.Mesh->GetScale();

		// �õ�3����������
		XMFLOAT3 RightVector = renderingData.Mesh->GetRightVector();
		XMFLOAT3 UpVector = renderingData.Mesh->GetUpVector();
		XMFLOAT3 ForwardVector = renderingData.Mesh->GetForwardVector();

		// ����ģ��world
		renderingData.MaterialTransformationMatrix = {
			RightVector.x * Scale.x,	UpVector.x,				ForwardVector.x,			0.f,
			RightVector.y,				UpVector.y * Scale.y,	ForwardVector.y,			0.f,
			RightVector.z,				UpVector.z,				ForwardVector.z * Scale.z,	0.f,
			Position.x,					Position.y,				Position.z,					1.f
		};

		XMMATRIX ATRTIXMatrixWorld = XMLoadFloat4x4(&renderingData.MaterialTransformationMatrix);
		XMMATRIX ATRTIXTextureWorld = XMLoadFloat4x4(&renderingData.TextureTransformationMatrix);

		FObjectTransformation OBJTransformation;
		// CPU�˴洢���������к��е�˳����GPU��Ĭ�������column_major�������෴
		// ��˵�����Ҫ��CPU�ľ���ͨ��Constant Buffer���ݵ�GPUʱ�������ڴ洢����ʱ���� �����ת�� ����
		XMStoreFloat4x4(&OBJTransformation.World, XMMatrixTranspose(ATRTIXMatrixWorld));
		XMStoreFloat4x4(&OBJTransformation.TextureTransformation, XMMatrixTranspose(ATRTIXTextureWorld));

		if (auto& material = (*renderingData.Mesh->GetMaterial())[0])
		{
			OBJTransformation.MaterialID = material->GetMaterialID();
			// ENGINE_LOG("����id = %d, ������ = %s", OBJTransformation.MaterialID, material->GetBaseColorIndexKey().c_str());
		}

		GeometryMap->MeshConstantBufferViews.Update(renderingData.MeshObjectIndex, &OBJTransformation);
	}
}

void FRenderLayer::BuildPSO()
{
	BuildShader();

	// ����PSO����
	DirectXPipelineState->BuildParam();
}


