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

bool FGeometry::bRenderingDataExistence(CMeshComponent* InKey)
{
	if (std::shared_ptr<FRenderLayer> renderLayer = FRenderLayerManage::FindByRenderLayer(InKey->GetRenderLayerType()))
	{
		for(const auto& temp : renderLayer->RenderData)
		{
			return temp.Mesh == InKey;
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
			renderLayer->RenderData.push_back(FRenderingData());

			FRenderingData& InRenderingData = renderLayer->RenderData[renderLayer->RenderData.size() - 1];

			InRenderingData.MeshObjectIndex = MeshObjectCount++;
			InRenderingData.GeometryKey = geometryKey;
			InRenderingData.Mesh = inMesh;
			InRenderingData.MeshHash = meshHash;
			// ��¼��������
			InRenderingData.IndexSize = MeshData.IndexData.size();
			InRenderingData.VertexSize = MeshData.VertexData.size();
			// ��¼����ƫ��
			InRenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();
			InRenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();

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

void FGeometry::DuplicateMesh(CMeshComponent* mesh_component, const FRenderingData& rendering_data, int geometryKey)
{
	// �жϵ�ǰģ���Ƿ��Ѿ�����ӹ���
	if (!bRenderingDataExistence(mesh_component))
	{
		if (std::shared_ptr<FRenderLayer> renderLayer = FRenderLayerManage::FindByRenderLayer(mesh_component->GetRenderLayerType()))
		{
			renderLayer->RenderData.push_back(rendering_data);
			FRenderingData& InRenderingData = renderLayer->RenderData[renderLayer->RenderData.size() - 1];

			InRenderingData.Mesh = mesh_component;
			InRenderingData.MeshObjectIndex = MeshObjectCount++;
			InRenderingData.GeometryKey = geometryKey;
		}
	}
}

bool FGeometry::FindMeshRenderingDataByHash(size_t hashKey, FRenderingData& rendering_data, int layer)
{
	auto findMeshRenderDataByHash = [&](std::shared_ptr<FRenderLayer> layers)->FRenderingData*
		{
			for (auto& tmp : layers->RenderData)
			{
				if (tmp.MeshHash == hashKey)
				{
					return &tmp;
				}
			}
			return nullptr;
		};

	// ȫ������������Ⱦ�㼶�����Ⱦ����
	if (layer == -1)
	{
		for (auto& layers : FRenderLayerManage::RenderLayers)
		{
			if (FRenderingData* renderData = findMeshRenderDataByHash(layers))
			{
				rendering_data = *renderData;
				return true;
			}
			return false;
		}
		return false;
	}
	else
	{
		// ����ָ������Ⱦ�㼶���в���
		if (std::shared_ptr<FRenderLayer> renderLayer = FRenderLayerManage::FindByRenderLayer(layer))
		{
			if (FRenderingData* renderData = findMeshRenderDataByHash(renderLayer))
			{
				rendering_data = *renderData;
				return true;
			}
		}
		return false;
	}
	
}

/**
 * \brief //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */
FGeometryMap::FGeometryMap() : IDirectXDeviceInterface_Struct(), Fog(nullptr)
{
	Geometries.insert(pair<int, FGeometry>(0, FGeometry()));

	RenderingTexture2DResourceViews = std::make_shared<FRenderingTextureResourcesUpdate>();
	RenderingTexture2DResourceViews->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURE2D);

	RenderingCubeMapResourceViews = std::make_shared<FRenderingTextureResourcesUpdate>();
	RenderingCubeMapResourceViews->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURECUBE);
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
	// +1 ��ʾ������ĳ��������� (ģ�Ͷ������� + �ƹ����� + ����� + ������ͼ + CubeMap��
	// DescriptorHeap.Build(GetDrawMeshCount() + GetDrawLightCount() + 1 + GetDrawTexture2DCount() + GetDrawCubeMapCount());

	// ������ͼ + cube map, ֻ��ҪΪtexture2DͼƬ������ڴ棬��Ϊ���ǽ�������(ģ�Ͷ������� + �ƹ����� + �����)�ӳ�����������������ˣ�ֻ�������cubemap���ڼ���ʹ��������
	DescriptorHeap.Build(GetDrawTexture2DCount() + GetDrawCubeMapCount());
}

UINT FGeometryMap::GetDrawTexture2DCount() const
{
	return RenderingTexture2DResourceViews->Size();
}

UINT FGeometryMap::GetDrawCubeMapCount() const
{
	return RenderingCubeMapResourceViews->Size();
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
	// �ռ�����
	for (const auto& layer : FRenderLayerManage::RenderLayers)
	{
		for (const auto& renderData : layer->RenderData)
		{
			if (const auto materials = renderData.Mesh->GetMaterial())
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

bool FGeometryMap::IsStartUpFog()
{
	return Fog != nullptr;
}

void FGeometryMap::BuildViewportConstantBuffer()
{
	// ��������������
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

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
				RenderingCubeMapResourceViews->LoadTextureResource(TexturePath);
			}
			else
			{
				// 2D����
				RenderingTexture2DResourceViews->LoadTextureResource(TexturePath);
			}
			
		}
	}
}

void FGeometryMap::BuildTextureConstBuffer()
{
	// ƫ�� = ģ����Ⱦ�� + �ƹ���Ⱦ�� + ������Ⱦ�� + �ӿ�
	// RenderingTexture2DResourceViews->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), GetDrawMeshCount() + GetDrawLightCount() + 1);

	// ��Ϊ��ģ�ͣ��ƹ⣬�ӿڶ��ӳ�����������������ˣ���������ƫ��ֱ�Ӹ�0����
	RenderingTexture2DResourceViews->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), 0);

	// ����CubeMap
	// ƫ�� = ģ����Ⱦ�� + �ƹ���Ⱦ�� + ������Ⱦ�� + �ӿ� + ��ͼ����
	RenderingCubeMapResourceViews->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), GetDrawTexture2DCount());
}

void FGeometryMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{

	// ���²���
	UpdateMaterialShaderResourceView(delta_time, viewport_info);

	XMMATRIX ProjectionMatrix = XMLoadFloat4x4(&viewport_info.ProjectionMatrix);	// ͶӰ����
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&viewport_info.ViewMatrix);				// �ӿھ���
	

	

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
			}
		}
	}
	LightConstantBufferViews.Update(0, &LightConstantBuffer);

	// �����ӿ�
	XMMATRIX ViewProjection = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);
	FViewportTransformation ViewportTransformation;
	ViewportTransformation.CameraPosition = viewport_info.CameraPosition;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProjection));	// �洢֮ǰ�ǵöԾ������ת��

	ViewportConstantBufferViews.Update(0, &ViewportTransformation);

	// ������
	if (Fog)
	{
		FFogConstantBuffer fogConstbuffer;

		fogConstbuffer.FogColor = Fog->GetFogColor();
		fogConstbuffer.FogStart = Fog->GetFogStart();
		fogConstbuffer.FogRange = Fog->GetFogRange();

		FogConstantBufferViews.Update(0, &fogConstbuffer);
	}
	
}

void FGeometryMap::UpdateMaterialShaderResourceView(float delta_time, const FViewportInfo& viewport_info)
{
	FMaterialConstantBuffer MaterialConstantBuffer = {};
	// ���²���
	for (auto& material :MaterialsSRV)
	{
		if (material->IsDirty())
		{
			MaterialConstantBuffer.MaterialType = material->GetMaterialType();
			MaterialConstantBuffer.BaseColor = material->GetBaseColor();
			MaterialConstantBuffer.Roughness = material->GetRoughness();
			MaterialConstantBuffer.SpecularColor = material->GetSpecularColor();

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

			material->SetDirty(false);

			MaterialConstantBufferViews.Update(material->GetMaterialID(), &MaterialConstantBuffer);
		}
	}
	
}

void FGeometryMap::PreDraw(float DeltaTime)
{
	DescriptorHeap.PreDraw(DeltaTime);
}

void FGeometryMap::Draw(float DeltaTime)
{
	// ��Ⱦ�ӿ�
	DrawViewport(DeltaTime);

	// ��Ⱦ�ƹ�
	DrawLight(DeltaTime);

	// ��Ⱦ��ͼ
	DrawTexture(DeltaTime);

	// ��Ⱦ����
	DrawMaterial(DeltaTime);

	// ��Ⱦģ��
	// DrawMesh(DeltaTime);

	// ��Ⱦ��
	DrawFog(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{
}

void FGeometryMap::DuplicateMesh(CMeshComponent* mesh_component, const FRenderingData& rendering_data)
{

	for (auto& geometry : Geometries)
	{
		geometry.second.DuplicateMesh(mesh_component, rendering_data, geometry.first);
	}
	
}

bool FGeometryMap::FindMeshRenderingDataByHash(size_t hashKey, FRenderingData& rendering_data, int layer)
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
	if (auto RenderTexturePtr = RenderingTexture2DResourceViews->FindRenderingTexture(key))
	{
		return RenderTexturePtr;
	}
	else if (auto RenderCubeMapPtr = RenderingCubeMapResourceViews->FindRenderingTexture(key))
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

void FGeometryMap::DrawTexture(float DeltaTime)
{
	// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	{
		CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

		DesHandle.Offset(0, DescriptorOffset);
		GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(5, DesHandle);
	}


	{
		CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

		DesHandle.Offset(GetDrawTexture2DCount(), DescriptorOffset);
		GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, DesHandle);
	}
}

void FGeometryMap::DrawFog(float DeltaTime)
{
	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(3, FogConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}
