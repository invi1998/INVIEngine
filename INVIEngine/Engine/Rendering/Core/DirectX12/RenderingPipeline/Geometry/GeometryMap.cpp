#include "EngineMinimal.h"

#include "GeometryMap.h"

#include <ranges>

#include "Component/Light/PointLightComponent.h"
#include "Component/Light/SpotLightComponent.h"
#include "Component/Light/Core/LightComponent.h"
#include "Component/Light/Core/LightConstantBuffer.h"
#include "Component/Sky/FogComponent.h"
#include "Component/Sky/FogConstantBuffer.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Core/Viewport/ViewportTransformation.h"
#include "Material/Core/Material.h"
#include "Material/Core/MaterialConstantBuffer.h"
#include "Mesh/Core/Mesh.h"
#include "Mesh/Core/ObjectTransformation.h"
#include "Rendering/Core/RenderingTextureResourcesUpdate.h"
#include "Rendering/Core/Buffer/ConstructBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"

UINT MeshObjectCount = 0;

// ������̬����
map<size_t, std::shared_ptr<FRenderingData>> FGeometry::RenderingDataPool{};
std::vector<std::shared_ptr<FRenderingData>> FGeometry::RenderingDataPoolVector{};

bool FGeometry::bRenderingDataExistence(CMeshComponent* InKey)
{
	if (std::shared_ptr<FRenderLayer> renderLayer = FRenderLayerManage::FindByRenderLayer(InKey->GetRenderLayerType()))
	{
		for(auto& temp : renderLayer->RenderData)
		{
			if (temp.expired())
			{
				continue;
			}

			if (std::shared_ptr<FRenderingData> renderData = temp.lock())
			{
				return renderData->Mesh == InKey;
			}
			
		}
	}
	

	return false;
}

void FGeometry::BuildMesh(const size_t meshHash, CMeshComponent* inMesh, const FMeshRenderingData& MeshData, int geometryKey)
{
	// �жϵ�ǰģ���Ƿ��Ѿ�����ӹ���
	if (!bRenderingDataExistence(inMesh))
	{
		if (std::shared_ptr<FRenderLayer> renderLayer = FRenderLayerManage::FindByRenderLayer(inMesh->GetRenderLayerType()))
		{
			RenderingDataPool.insert(pair<size_t, std::shared_ptr<FRenderingData>>(meshHash, std::make_shared<FRenderingData>()));

			RenderingDataPoolVector.push_back(std::make_shared<FRenderingData>());

			std::shared_ptr<FRenderingData> InRenderingData = RenderingDataPoolVector[RenderingDataPoolVector.size() - 1];

			renderLayer->RenderData.push_back(InRenderingData);

			// ���ģ�͵�AABB��Χ��
			XMFLOAT3 Min = { FLT_MAX, FLT_MAX, FLT_MAX };			// ��С��
			XMFLOAT3 Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };		// ����

			for (auto& vertex : MeshData.VertexData)
			{
				Min.x = min(Min.x, vertex.Position.x);
				Min.y = min(Min.y, vertex.Position.y);
				Min.z = min(Min.z, vertex.Position.z);

				Max.x = max(Max.x, vertex.Position.x);
				Max.y = max(Max.y, vertex.Position.y);
				Max.z = max(Max.z, vertex.Position.z);
			}

			InRenderingData->Bounds.Center = XMFLOAT3((Min.x + Max.x) / 2.f, (Min.y + Max.y) / 2.f, (Min.z + Max.z) / 2.f);
			InRenderingData->Bounds.Extents = XMFLOAT3((Max.x - Min.x) / 2.f, (Max.y - Min.y) / 2.f, (Max.z - Min.z) / 2.f);

			InRenderingData->MeshObjectIndex = MeshObjectCount++;
			InRenderingData->GeometryKey = geometryKey;
			InRenderingData->Mesh = inMesh;
			InRenderingData->MeshHash = meshHash;
			// ��¼��������
			InRenderingData->IndexSize = MeshData.IndexData.size();
			InRenderingData->VertexSize = MeshData.VertexData.size();
			// ��¼����ƫ��
			InRenderingData->IndexOffsetPosition = MeshRenderingData.IndexData.size();
			InRenderingData->VertexOffsetPosition = MeshRenderingData.VertexData.size();

			InRenderingData->MeshRenderingData = &MeshRenderingData;

			RenderingDataPool[meshHash]->MeshObjectIndex = InRenderingData->MeshObjectIndex;
			RenderingDataPool[meshHash]->GeometryKey = InRenderingData->GeometryKey;
			RenderingDataPool[meshHash]->Mesh = InRenderingData->Mesh;
			RenderingDataPool[meshHash]->MeshHash = InRenderingData->MeshHash;

			RenderingDataPool[meshHash]->IndexSize = MeshData.IndexData.size();
			RenderingDataPool[meshHash]->VertexSize = MeshData.VertexData.size();
			RenderingDataPool[meshHash]->IndexOffsetPosition = MeshRenderingData.IndexData.size();
			RenderingDataPool[meshHash]->VertexOffsetPosition = MeshRenderingData.VertexData.size();

			RenderingDataPool[meshHash]->MeshRenderingData = &MeshRenderingData;
			RenderingDataPool[meshHash]->Bounds = InRenderingData->Bounds;

			// һ�ָ�Ч�����ݲ��뷽ʽ
			// �����ϲ�
			MeshRenderingData.IndexData.insert(MeshRenderingData.IndexData.end(), MeshData.IndexData.begin(), MeshData.IndexData.end());
			// ����ϲ�
			MeshRenderingData.VertexData.insert(MeshRenderingData.VertexData.end(), MeshData.VertexData.begin(), MeshData.VertexData.end());
		}
	}
}

void FGeometry::Build()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����ģ��

	const UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	const UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();

	// ����������
	ANALYSIS_RESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));	// ����һ�������ƵĻ�����
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), MeshRenderingData.VertexData.data(), VertexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), MeshRenderingData.IndexData.data(), IndexSizeInBytes);

	ConstructBuffer::FConstructDefaultBuffer ConstructDefaultBuffer;

	GPUVertexBufferPtr = ConstructDefaultBuffer.ConstructDefaultBuffer(TempVertexBufferPtr, MeshRenderingData.VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer.ConstructDefaultBuffer(TempIndexBufferPtr, MeshRenderingData.IndexData.data(), IndexSizeInBytes);

}

UINT FGeometry::GetDrawObjectCount() const
{
	return MeshObjectCount;

	UINT size = 0;
	for (auto& layers : FRenderLayerManage::RenderLayers)
	{
		size += layers->RenderData.size();
	}
	return size;
}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW vbv;

	vbv.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	vbv.SizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	vbv.StrideInBytes = sizeof(FVertex);

	return vbv;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW ibv;

	ibv.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
	ibv.SizeInBytes = MeshRenderingData.GetIndexSizeInBytes();
	ibv.Format = DXGI_FORMAT_R16_UINT;

	return ibv;
}

void FGeometry::DuplicateMesh(CMeshComponent* mesh_component, std::shared_ptr<FRenderingData>& rendering_data, int geometryKey)
{
	// �жϵ�ǰģ���Ƿ��Ѿ�����ӹ���
	if (std::shared_ptr<FRenderLayer> inRenderLayer = FRenderLayerManage::FindByRenderLayer(mesh_component->GetRenderLayerType()))
	{
		RenderingDataPoolVector.push_back(std::make_shared<FRenderingData>());
		std::shared_ptr<FRenderingData> InRenderingData = RenderingDataPoolVector[RenderingDataPoolVector.size() - 1];

		inRenderLayer->RenderData.push_back(InRenderingData);

		InRenderingData->MeshObjectIndex = MeshObjectCount++;
		InRenderingData->Mesh = mesh_component;
		InRenderingData->MeshHash = rendering_data->MeshHash;
		InRenderingData->GeometryKey = geometryKey;

		InRenderingData->IndexSize = rendering_data->IndexSize;
		InRenderingData->VertexSize = rendering_data->VertexSize;
		InRenderingData->IndexOffsetPosition = rendering_data->IndexOffsetPosition;
		InRenderingData->VertexOffsetPosition = rendering_data->VertexOffsetPosition;

		InRenderingData->MeshRenderingData = &MeshRenderingData;
		
		InRenderingData->Bounds = rendering_data->Bounds;
	}
}

bool FGeometry::FindMeshRenderingDataByHash(size_t hashKey, std::shared_ptr<FRenderingData>& rendering_data, int layer)
{
	auto findResult = RenderingDataPool.find(hashKey);
	if (findResult != RenderingDataPool.end())
	{
		rendering_data = findResult->second;
		return true;
	}
	return false;
}

void FGeometry::FindRenderingData(std::function<EFindValueType(std::shared_ptr<FRenderingData>&)> func)
{
	for (auto& temp : RenderingDataPoolVector)
	{
		if (func(temp) == EFindValueType::FVT_COMPLETE)
		{
			return;
		}
	}
}

/**
 * \brief //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */
FGeometryMap::FGeometryMap() : IDirectXDeviceInterface_Struct(), Fog(nullptr)
{
	Geometries.insert(pair<int, FGeometry>(0, FGeometry()));

	RenderingTexture2DResources = std::make_shared<FRenderingTextureResourcesUpdate>();
	RenderingTexture2DResources->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURE2D);

	RenderingCubeMapResources = std::make_shared<FRenderingTextureResourcesUpdate>();
	RenderingCubeMapResources->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURECUBE);
}

FGeometryMap::~FGeometryMap()
{
	MeshObjectCount = 0;
}

void FGeometryMap::BuildMesh(const size_t meshHash, CMeshComponent* Mesh, const FMeshRenderingData& MeshData)
{
	for (auto& geometry:Geometries)
	{
		geometry.second.BuildMesh(meshHash, Mesh, MeshData, geometry.first);
	}

	
}

void FGeometryMap::Build()
{
	for (auto& Geometry : Geometries | views::values)
	{
		Geometry.Build();
	}
}

void FGeometryMap::BuildDescriptorHeap()
{
	// +1 ��ʾ������ĳ��������� (ģ�Ͷ������� + �ƹ����� + ����� + ������ͼ + CubeMap + ShadowMap��
	// DescriptorHeap.Build(GetDrawMeshCount() + GetDrawLightCount() + 1 + GetDrawTexture2DCount() + GetDrawCubeMapCount());

	// ������ͼ + cube map(��̬cubeMap) + CubeMapShadow + ui, ֻ��ҪΪtexture2DͼƬ������ڴ棬��Ϊ���ǽ�������(ģ�Ͷ������� + �ƹ����� + �����)�ӳ�����������������ˣ�ֻ�������cubemap���ڼ���ʹ��������
	DescriptorHeap.Build(GetDrawTexture2DCount() + // ������ͼ����
		GetDrawCubeMapCount() +	// CubeMap����
		1 + // ����Cubemap ��̬����
		1 +	// ��Ӱ��ͼ ֱ��ƣ��۹��
		1 + // shadowCubeMap 6���� (���Դ��Ӱ��
		1 + // UI
		1 +	// ����
		1 + // ���
		1	// SSAO
	); 
}

UINT FGeometryMap::GetDrawTexture2DCount() const
{
	return RenderingTexture2DResources->Size();
}

UINT FGeometryMap::GetDrawCubeMapCount() const
{
	return RenderingCubeMapResources->Size();
}

UINT FGeometryMap::GetDrawMeshCount()
{
	// Ŀǰ��д���ɵ�һ��
	return Geometries[0].GetDrawObjectCount();
}

UINT FGeometryMap::GetDrawMaterialCount()
{
	return MaterialsSRV.size();
}

UINT FGeometryMap::GetDrawLightCount()
{
	return 1;
}

UINT FGeometryMap::GetDynamicReflectionViewportNum()
{
	return DynamicReflectionMeshComponents.size() * 6;
}

void FGeometryMap::BuildMeshConstantBuffer()
{
	// ��������������
	MeshConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshCount());

	//// �����Ѿ��
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//// ��������������
	//MeshConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawMeshCount());
}

void FGeometryMap::BuildMaterialShaderResourceView()
{
	for (const auto& renderData : FGeometry::RenderingDataPoolVector)
	{
		if (const auto& materials = renderData->Mesh->GetMaterial())
		{
			for (size_t i = 0; i < materials->size(); i++)
			{
				// ���ò���ID
				(*materials)[i]->SetMaterialID(MaterialsSRV.size());
				// �������
				MaterialsSRV.push_back((*materials)[i]);
			}
		}
		
	}

	// ��������������
	MaterialConstantBufferViews.CreateConstant(sizeof(FMaterialConstantBuffer), GetDrawMaterialCount(), false);
}

void FGeometryMap::BuildLightConstantBuffer()
{
	// �����ƹⳣ��������
	LightConstantBufferViews.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightCount());

	//// �����Ѿ��
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//// ��������������
	//LightConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawLightCount(), GetDrawMeshCount());
}

void FGeometryMap::BuildFogConstantBuffer()
{
	// ������ĳ���������
	FogConstantBufferViews.CreateConstant(sizeof(FFogConstantBuffer), 1);
}

void FGeometryMap::BuildFog()
{
	for (const auto & tmp : GObjects)
	{
		if (CFogComponent* fogComponent = dynamic_cast<CFogComponent*>(tmp))
		{
			Fog = fogComponent;
			return;
		}
	}
}

void FGeometryMap::BuildShadow()
{
	DynamicShadowMap.ResetView(2048, 2048);
	DynamicShadowMap.BuildViewPort(XMFLOAT3{ 15.f, 12.f, 0.f });
	DynamicShadowMap.BuildDepthStencilViewDesc();
	DynamicShadowMap.BuildShadowMapRenderTargetDescriptor();

	DynamicShadowCubeMap.BuildViewPort(XMFLOAT3{ 0.f, 0.f, 0.f });
	DynamicShadowCubeMap.BuildDepthStencilDescriptor();
	DynamicShadowCubeMap.BuildCubeMapRenderTargetDescriptor();
	DynamicShadowCubeMap.BuildDepthStencil();
}

bool FGeometryMap::IsStartUpFog()
{
	return Fog != nullptr;
}

void FGeometryMap::BuildViewportConstantBuffer(UINT viewPortOffset)
{
	// �������������� (���ӿ� + ��̬�����ӿ� + shadow������ӿ� + ���ԴCubeMapShadow�ӿڣ�
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1 + GetDynamicReflectionViewportNum() + 1 + 6 + viewPortOffset);

	//// �����Ѿ��
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//// ��������������
	//ViewportConstantBufferViews.BuildConstantBuffer(DesHandle, 1, GetDrawMeshCount() + GetDrawLightCount());
}

void FGeometryMap::LoadTexture() const
{
	def_c_paths Paths;
	init_def_c_paths(&Paths);

	char RootPath[] = "Asserts/Texture";
	find_files(RootPath, &Paths, true, true);

	for (int i = 0; i < Paths.index; i++)
	{
		if (find_string(Paths.paths[i], ".dds", 0) != -1)
		{
			// ��λ��·��
			normalization_path(Paths.paths[i]);

			wchar_t TexturePath[1024] = { 0 };
			char_to_wchar_t(TexturePath, 1024, Paths.paths[i]);

			if (wfind_string(TexturePath, L"_CubeMap.")!=-1 || wfind_string(TexturePath, L"_cubemap.") != -1)
			{
				// cube map
				RenderingCubeMapResources->LoadTextureResource(TexturePath);
			}
			else
			{
				// 2D����
				RenderingTexture2DResources->LoadTextureResource(TexturePath);
			}
			
		}
	}
}

void FGeometryMap::BuildTextureConstBuffer()
{
	// ƫ�� = ģ����Ⱦ�� + �ƹ���Ⱦ�� + ������Ⱦ�� + �ӿ�
	// RenderingTexture2DResources->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), GetDrawMeshCount() + GetDrawLightCount() + 1);

	// ��Ϊ��ģ�ͣ��ƹ⣬�ӿڶ��ӳ�����������������ˣ���������ƫ��ֱ�Ӹ�0����
	RenderingTexture2DResources->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), 0);

	// ����CubeMap
	// ƫ�� = ģ����Ⱦ�� + �ƹ���Ⱦ�� + ������Ⱦ�� + �ӿ� + ��ͼ����
	RenderingCubeMapResources->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), GetDrawTexture2DCount());
}

void FGeometryMap::BuildDynamicReflectionMeshComponent()
{
	for (const auto& tmp : GObjects)
	{
		if (CMeshComponent* component = dynamic_cast<CMeshComponent*>(tmp))
		{
			if (component->IsDynamicReflection())
			{
				DynamicReflectionMeshComponents.push_back(component);
			}
		}
	}
}

void FGeometryMap::UpdateCalculationViewport(const FViewportInfo& viewport_info, UINT constBufferOffset)
{
	XMMATRIX ProjectionMatrix = XMLoadFloat4x4(&viewport_info.ProjectionMatrix);	// ͶӰ����
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&viewport_info.ViewMatrix);				// �ӿھ���

	// �����ӿ�
	XMMATRIX ViewProjection = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);
	FViewportTransformation ViewportTransformation;
	ViewportTransformation.CameraPosition = viewport_info.CameraPosition;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProjection));	// �洢֮ǰ�ǵöԾ������ת��

	ViewportConstantBufferViews.Update(constBufferOffset, &ViewportTransformation);
}

void FGeometryMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{

	// ���²���
	UpdateMaterialShaderResourceView(delta_time, viewport_info);

	// ���µƹ�
	UpdateLight(delta_time, viewport_info);

	// �����ӿ�
	UpdateCalculationViewport(viewport_info, 0);

	// ������
	UpdateFog(delta_time, viewport_info);

	// ������Ӱ
	UpdateShadow(delta_time, viewport_info);
	
}

void FGeometryMap::UpdateMaterialShaderResourceView(float delta_time, const FViewportInfo& viewport_info)
{
	FMaterialConstantBuffer MaterialConstantBuffer = {};
	// ���²���
	for (auto& material : MaterialsSRV)
	{
		if (material->IsDirty())
		{
			MaterialConstantBuffer.MaterialType = material->GetMaterialType();	// ��������
			MaterialConstantBuffer.BaseColor = material->GetBaseColor();	// ������ɫ
			MaterialConstantBuffer.Roughness = material->GetRoughness();	// �ֲڶ�
			MaterialConstantBuffer.SpecularColor = material->GetSpecularColor();	// �߹���ɫ
			MaterialConstantBuffer.FresnelF0 = material->GetFresnelF0();		// ������F0
			MaterialConstantBuffer.Transparency = material->GetTransparency();		// ͸����
			MaterialConstantBuffer.RefractiveValue = material->GetRefractiveValue();	// ������
			MaterialConstantBuffer.Metallicity = material->GetMetallicity();		// ������

			if (auto basecolorPtr = FindRenderingTexture(material->GetBaseColorIndexKey()))
			{
				MaterialConstantBuffer.BaseColorIndex = (*basecolorPtr)->RenderingTextureID;
				ENGINE_LOG("���²��� �������� = %d, ������ = %ls", MaterialConstantBuffer.BaseColorIndex, (*basecolorPtr)->Name.c_str());
			}
			else
			{
				MaterialConstantBuffer.BaseColorIndex = -1;
			}

			// ������ͼ
			if (auto normalColor = FindRenderingTexture(material->GetNormalIndexKey()))
			{
				MaterialConstantBuffer.NormalIndex = (*normalColor)->RenderingTextureID;
				ENGINE_LOG("���·�����ͼ �������� = %d, ������ = %ls", MaterialConstantBuffer.NormalIndex, (*normalColor)->Name.c_str());
			}
			else
			{
				MaterialConstantBuffer.NormalIndex = -1;
			}

			// �߹���ͼ
			if (auto specularColor = FindRenderingTexture(material->GetSpecularIndexKey()))
			{
				MaterialConstantBuffer.SpecularIndex = (*specularColor)->RenderingTextureID;
				ENGINE_LOG("���¸߹���ͼ �������� = %d, ������ = %ls", MaterialConstantBuffer.SpecularIndex, (*specularColor)->Name.c_str());
			}
			else
			{
				MaterialConstantBuffer.SpecularIndex = -1;
			}

			XMFLOAT4X4 MaterialTransform = material->GetTransformation();
			XMMATRIX Transform = XMLoadFloat4x4(&MaterialTransform);
			// ����������о���תΪ�о�����shader��
			XMStoreFloat4x4(&MaterialConstantBuffer.Transformation, XMMatrixTranspose(Transform));

			MaterialConstantBuffer.Param0 = material->GetParam(0);
			MaterialConstantBuffer.Param1 = material->GetParam(1);
			MaterialConstantBuffer.Param2 = material->GetParam(2);

			material->SetDirty(false);

			MaterialConstantBufferViews.Update(material->GetMaterialID(), &MaterialConstantBuffer);
		}
	}
	
}

void FGeometryMap::UpdateLight(float delta_time, const FViewportInfo& viewport_info)
{
	// ���µƹ�
	FLightConstantBuffer LightConstantBuffer;
	for (size_t i = 0; i < GetLightManger()->Lights.size(); i++)
	{
		if (CLightComponent* lightComponent = GetLightManger()->Lights[i])
		{
			LightConstantBuffer.SceneLights[i].LightDirection = lightComponent->GetForwardVector();
			LightConstantBuffer.SceneLights[i].LightIntensity = lightComponent->GetLightIntensity();
			LightConstantBuffer.SceneLights[i].LightType = static_cast<int>(lightComponent->GetLightType());
			LightConstantBuffer.SceneLights[i].LightPosition = lightComponent->GetPosition();

			switch (lightComponent->GetLightType())
			{
			case DirectionalLight:
				{
					// ƽ�й� ���ﹹ��������������

					DynamicShadowMap.BuildOrthoProjectionMatrix({ 0, 0, 0 },lightComponent->GetForwardVector(), 70.f);

					// ��ȡViewMatrix
					XMFLOAT4X4 ViewMatrix = DynamicShadowMap.GetViewMatrix();

					// ��ȡProjectionMatrix
					XMFLOAT4X4 ProjectionMatrix = DynamicShadowMap.GetProjectionMatrix();

					XMMATRIX ShadowViewMatrix = XMLoadFloat4x4(&ViewMatrix);
					XMMATRIX ShadowProjectionMatrix = XMLoadFloat4x4(&ProjectionMatrix);

					// ׼��һ���任���� �þ�����Խ�����NDC�ռ����[-1, 1]ת����[0, 1]
					XMMATRIX Transform = {
						0.5f, 0.0f, 0.0f, 0.0f,
						0.0f, -0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.5f, 0.5f, 0.0f, 1.0f
					};

					// NDC [-1, 1] ==> [0, 1]
					XMMATRIX ShadowViewProjectionMatrix = ShadowViewMatrix * ShadowProjectionMatrix * Transform;

					// �洢Shadow�仯���� (�ǵ�Ҫ�ȶԾ������ת��)
					XMStoreFloat4x4(&LightConstantBuffer.SceneLights[i].ShadowTransform, XMMatrixTranspose(ShadowViewProjectionMatrix));
					break;
				};
			case PointLight:
				{
					if (auto rangeLightComponent = dynamic_cast<CRangeLightComponent*>(lightComponent))
					{
						LightConstantBuffer.SceneLights[i].StartAttenuation = rangeLightComponent->GetStartAttenuation();
						LightConstantBuffer.SceneLights[i].EndAttenuation = rangeLightComponent->GetEndAttenuation();
						LightConstantBuffer.SceneLights[i].Kc = rangeLightComponent->GetKc();
						LightConstantBuffer.SceneLights[i].Kl = rangeLightComponent->GetKl();
						LightConstantBuffer.SceneLights[i].Kq = rangeLightComponent->GetKq();
					}
					break;
				};
			case SpotLight:
				{
					if (auto rangeLightComponent = dynamic_cast<CRangeLightComponent*>(lightComponent))
					{
						LightConstantBuffer.SceneLights[i].StartAttenuation = rangeLightComponent->GetStartAttenuation();
						LightConstantBuffer.SceneLights[i].EndAttenuation = rangeLightComponent->GetEndAttenuation();
						LightConstantBuffer.SceneLights[i].Kc = rangeLightComponent->GetKc();
						LightConstantBuffer.SceneLights[i].Kl = rangeLightComponent->GetKl();
						LightConstantBuffer.SceneLights[i].Kq = rangeLightComponent->GetKq();
					}
					if (auto spotLightComponent = dynamic_cast<CSpotLightComponent*>(lightComponent))
					{
						LightConstantBuffer.SceneLights[i].SpotInnerCornerPhi = math_utils::angle_to_radian(spotLightComponent->GetSpotInnerCornerPhi());
						LightConstantBuffer.SceneLights[i].SpotOuterCornerTheta = math_utils::angle_to_radian(spotLightComponent->GetSpotOuterCornerTheta());

						// �۹�� ���ﹹ������͸��ͶӰ����

						DynamicShadowMap.BuildPerspectiveProjectionMatrix(lightComponent->GetPosition(), lightComponent->GetForwardVector(), 70.f);

						// ��ȡViewMatrix
						XMFLOAT4X4 ViewMatrix = DynamicShadowMap.GetViewMatrix();

						// ��ȡProjectionMatrix
						XMFLOAT4X4 ProjectionMatrix = DynamicShadowMap.GetProjectionMatrix();

						XMMATRIX ShadowViewMatrix = XMLoadFloat4x4(&ViewMatrix);
						XMMATRIX ShadowProjectionMatrix = XMLoadFloat4x4(&ProjectionMatrix);

						// ׼��һ���任���� �þ�����Խ�����NDC�ռ����[-1, 1]ת����[0, 1]
						XMMATRIX Transform = {
							0.5f, 0.0f, 0.0f, 0.0f,
							0.0f, -0.5f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.5f, 0.5f, 0.0f, 1.0f
						};

						// NDC [-1, 1] ==> [0, 1]
						XMMATRIX ShadowViewProjectionMatrix = ShadowViewMatrix * ShadowProjectionMatrix * Transform;

						// �洢Shadow�仯���� (�ǵ�Ҫ�ȶԾ������ת��)
						XMStoreFloat4x4(&LightConstantBuffer.SceneLights[i].ShadowTransform, XMMatrixTranspose(ShadowViewProjectionMatrix));
					}

					break;
				};
			default: break;;
			}

			
		}
	}
	LightConstantBufferViews.Update(0, &LightConstantBuffer);
}

void FGeometryMap::UpdateShadow(float delta_time, const FViewportInfo& viewport_info)
{
	// ������Ӱ
	DynamicShadowMap.UpdateCalculations(delta_time, viewport_info);
}

void FGeometryMap::UpdateFog(float delta_time, const FViewportInfo& viewport_info)
{
	// ������
	if (Fog)
	{
		if (Fog->IsDirty())
		{
			FFogConstantBuffer fogConstbuffer;

			fogConstbuffer.FogColor = Fog->GetFogColor();
			fogConstbuffer.FogStart = Fog->GetFogStart();
			fogConstbuffer.FogRange = Fog->GetFogRange();
			fogConstbuffer.FogHeight = Fog->GetFogHeight();
			fogConstbuffer.FogTransparentCoefficient = Fog->GetFogTransparentCoefficient();

			// fogConstbuffer.FogTransparentCoefficient = Fog->GetFogTransparentCoefficient();

			FogConstantBufferViews.Update(0, &fogConstbuffer);

			Fog->SetDirty(false);
		}

	}
}

void FGeometryMap::PreDraw(float DeltaTime)
{
	DescriptorHeap.PreDraw(DeltaTime);
}

void FGeometryMap::Draw(float DeltaTime)
{
	// ��Ⱦ�ƹ�
	DrawLight(DeltaTime);

	// ��Ⱦ��ͼ
	DrawTexture2D(DeltaTime);

	// ��ȾCubeMap (��������ͼ��
	DrawCubeMapTexture(DeltaTime);

	// ��Ⱦ����
	DrawMaterial(DeltaTime);

	// ��Ⱦ��
	DrawFog(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{
}

void FGeometryMap::DuplicateMesh(CMeshComponent* mesh_component, std::shared_ptr<FRenderingData>& rendering_data)
{

	for (auto& geometry : Geometries)
	{
		geometry.second.DuplicateMesh(mesh_component, rendering_data, geometry.first);
	}
	
}

bool FGeometryMap::FindMeshRenderingDataByHash(size_t hashKey, std::shared_ptr<FRenderingData>& rendering_data, int layer)
{
	for (auto& geometry : Geometries)
	{
		if (geometry.second.FindMeshRenderingDataByHash(hashKey, rendering_data, layer))
		{
			return true;
		}
	}
	return false;
}

std::unique_ptr<FRenderingTexture>* FGeometryMap::FindRenderingTexture(const std::string& key)
{
	if (auto RenderTexturePtr = RenderingTexture2DResources->FindRenderingTexture(key))
	{
		return RenderTexturePtr;
	}
	else if (auto RenderCubeMapPtr = RenderingCubeMapResources->FindRenderingTexture(key))
	{
		return RenderCubeMapPtr;
	}
	return nullptr;
}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	//// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	//UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	//DesHandle.Offset(static_cast<INT>(GetDrawMeshCount() + GetDrawLightCount()), DescriptorOffset);
	//GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);		// ����b1�Ĵ��� (�ӿ��Ǻ���Ⱦ�����Է��ڼĴ���1�У�


	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());

}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	// GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(2, LightConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawMaterial(float DeltaTime)
{

	GetD3dGraphicsCommandList()->SetGraphicsRootShaderResourceView(
		4,	// Ҫ�󶨵ĸ�ǩ���۵�����
		MaterialConstantBufferViews.GetBuffer()->GetGPUVirtualAddress()	// ��ɫ����Դ��ͼ�� GPU �����������
		);
}

void FGeometryMap::DrawLight(float DeltaTime)
{
	//// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	//UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	//DesHandle.Offset(static_cast<INT>(GetDrawMeshCount()), DescriptorOffset);
	//GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, DesHandle);		// ����b1�Ĵ��� (�ӿ��Ǻ���Ⱦ�����Է��ڼĴ���1�У�

	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(2, LightConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawShadow(float DeltaTime)
{
	DynamicShadowMap.Draw(DeltaTime);
}

void FGeometryMap::DrawTexture2D(float DeltaTime)
{
	// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(0, DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(5, DesHandle);

}

void FGeometryMap::DrawCubeMapTexture(float DeltaTime)
{
	// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(static_cast<int>(GetDrawTexture2DCount()), DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, DesHandle);
}

void FGeometryMap::DrawFog(float DeltaTime)
{
	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(3, FogConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

D3D12_GPU_VIRTUAL_ADDRESS FGeometryMap::GetViewportConstantBufferViewsGPUVirtualAddr()
{
	return ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();
}
