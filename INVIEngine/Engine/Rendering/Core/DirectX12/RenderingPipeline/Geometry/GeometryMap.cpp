#include "EngineMinimal.h"

#include "GeometryMap.h"

#include <ranges>

#include "Component/Light/PointLightComponent.h"
#include "Component/Light/SpotLightComponent.h"
#include "Component/Light/Core/LightComponent.h"
#include "Component/Light/Core/LightConstantBuffer.h"
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

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));	// ����һ�������ƵĻ�����
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
FGeometryMap::FGeometryMap()
{
	Geometries.insert(pair<int, FGeometry>(0, FGeometry()));

	RenderingTextureResourceViews = std::make_shared<FRenderingTextureResourcesUpdate>();
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
	for (auto & [index, Geometry] : Geometries)
	{
		Geometry.Build();
	}
}

void FGeometryMap::BuildDescriptorHeap()
{
	// +1 ��ʾ������ĳ��������� (ģ�Ͷ������� + �ƹ����� + ����� + ������ͼ��
	DescriptorHeap.Build(GetDrawMeshCount() + GetDrawLightCount() + 1 + GetDrawTextureCount());
}

UINT FGeometryMap::GetDrawTextureCount()
{
	return RenderingTextureResourceViews->Size();
}

UINT FGeometryMap::GetDrawMeshCount()
{
	// Ŀǰ��д���ɵ�һ��
	return Geometries[0].GetDrawObjectCount();
}

UINT FGeometryMap::GetDrawMaterialCount()
{
	UINT MaterialNumber = 0;
	for (auto& geometry : Geometries)
	{
		for (auto& renderData : geometry.second.DescribeMeshRenderingData)
		{
			MaterialNumber += renderData.Mesh->GetMaterialNum();
		}
	}

	return MaterialNumber;
}

UINT FGeometryMap::GetDrawLightCount()
{
	return 1;
}

void FGeometryMap::BuildMeshConstantBuffer()
{
	// ��������������
	MeshConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshCount());

	// �����Ѿ��
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	// ��������������
	MeshConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawMeshCount());
}

void FGeometryMap::BuildMaterialShaderResourceView()
{
	// ��������������
	MaterialConstantBufferViews.CreateConstant(sizeof(FMaterialConstantBuffer), GetDrawMaterialCount(), false);

	//for (auto& Tmp : Geometries)
	//{
	//	for (size_t i = 0; i < Tmp.second.DescribeMeshRenderingData.size(); i++)
	//	{
	//		auto& InData = Tmp.second.DescribeMeshRenderingData[i];
	//		if (auto InMaterials = InData.Mesh->GetMaterial())
	//		{
	//			for (size_t j = 0; j < InMaterials->size(); j++)
	//			{
	//				//��ShaderIndex����
	//				(*InMaterials)[j]->SetMaterialID(MaterialsSRV.size());

	//				MaterialsSRV.push_back((*InMaterials)[j]);
	//			}
	//		}
	//	}
	//}

	// �ռ�����
	for (auto& geometry : Geometries)
	{
		for (auto& renderData: geometry.second.DescribeMeshRenderingData)
		{
			if (auto materials = renderData.Mesh->GetMaterial())
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
}

void FGeometryMap::BuildLightConstantBuffer()
{
	// �����ƹⳣ��������
	LightConstantBufferViews.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightCount());

	// �����Ѿ��
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	// ��������������
	LightConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawLightCount(), GetDrawMeshCount());
}

void FGeometryMap::BuildViewportConstantBuffer()
{
	// ��������������
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

	// �����Ѿ��
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	// ��������������
	ViewportConstantBufferViews.BuildConstantBuffer(DesHandle, 1, GetDrawMeshCount() + GetDrawLightCount());
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
			RenderingTextureResourceViews->LoadTextureResource(TexturePath);
		}
	}
}

void FGeometryMap::BuildTextureConstBuffer()
{
	// ƫ�� = ģ����Ⱦ�� + �ƹ���Ⱦ�� + ������Ⱦ�� + �ӿ�
	RenderingTextureResourceViews->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), GetDrawMeshCount() + GetDrawLightCount() + 1);
}

void FGeometryMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	XMMATRIX ProjectionMatrix = XMLoadFloat4x4(&viewport_info.ProjectionMatrix);	// ͶӰ����
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&viewport_info.ViewMatrix);				// �ӿھ���
	for (auto& geometry : Geometries)
	{
		for (int i = 0; i < geometry.second.DescribeMeshRenderingData.size(); i++)
		{
			// ����ģ��
			FRenderingData& renderingData = geometry.second.DescribeMeshRenderingData[i];
			{
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
					RightVector.z,				UpVector.z,				ForwardVector.z	* Scale.z,	0.f,
					Position.x,					Position.y,				Position.z,					1.f
				};
			}
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

			MeshConstantBufferViews.Update(i, &OBJTransformation);

		}
	}

	// ���²���
	UpdateMaterialShaderResourceView(delta_time, viewport_info);

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

			if (auto basecolorPtr = RenderingTextureResourceViews->FindRenderingTexture(material->GetBaseColorIndexKey()))
			{
				MaterialConstantBuffer.BaseColorIndex = (*basecolorPtr)->RenderingTextureID;
				ENGINE_LOG("���²��� �������� = %d, ������ = %ls", MaterialConstantBuffer.BaseColorIndex, (*basecolorPtr)->Name.c_str());
			}
			else
			{
				MaterialConstantBuffer.BaseColorIndex = -1;
			}

			// ������ͼ
			if (auto normalColor = RenderingTextureResourceViews->FindRenderingTexture(material->GetNormalIndexKey()))
			{
				MaterialConstantBuffer.NormalIndex = (*normalColor)->RenderingTextureID;
				ENGINE_LOG("���·�����ͼ �������� = %d, ������ = %ls", MaterialConstantBuffer.NormalIndex, (*normalColor)->Name.c_str());
			}
			else
			{
				MaterialConstantBuffer.NormalIndex = -1;
			}

			// �߹���ͼ
			if (auto specularColor = RenderingTextureResourceViews->FindRenderingTexture(material->GetSpecularIndexKey()))
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

void FGeometryMap::DrawViewport(float DeltaTime)
{
	// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(static_cast<INT>(GetDrawMeshCount() + GetDrawLightCount()), DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);		// ����b1�Ĵ��� (�ӿ��Ǻ���Ⱦ�����Է��ڼĴ���1�У�
}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (auto& geometry : Geometries | views::values)
	{
		// ��ȡ����/������������ͼ
		D3D12_VERTEX_BUFFER_VIEW vbv = geometry.GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW ibv = geometry.GetIndexBufferView();

		for (size_t i = 0; i < geometry.DescribeMeshRenderingData.size(); i++)
		{
			CD3DX12_GPU_DESCRIPTOR_HANDLE DesMeshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());	// ģ������handle

			FRenderingData& renderingData = geometry.DescribeMeshRenderingData[i];

			// ����������ͼ
			GetD3dGraphicsCommandList()->IASetIndexBuffer(&ibv);

			// ����Ⱦ��ˮ���ϵ�����ۣ�����������װ�����׶δ��붥������
			GetD3dGraphicsCommandList()->IASetVertexBuffers(
				0,		// ָ����ʼ�Ķ��㻺�����ۺ� `StartSlot`�����ֵͨ��Ϊ 0 ֵ��Χ 0-15
				1,		// ָ��Ҫ�󶨵Ķ��㻺�������� NumViews�����ֵ����С�ڻ����Ӳ��֧�ֵ���󶥵㻺��������
				&vbv	// ʹ�ö��㻺������ͼ���� pViews ������ÿ�����㻺������λ�á���С�Ͳ��ֵ���Ϣ��
			);

			// ָ��ͼԪ���ͣ��㣬�ߣ��棩 ��������Ϊ �����������ι��ɵ��б�
			D3D_PRIMITIVE_TOPOLOGY DisplayType = static_cast<D3D_PRIMITIVE_TOPOLOGY>((*renderingData.Mesh->GetMaterial())[0]->GetMaterialDisplayType());
			GetD3dGraphicsCommandList()->IASetPrimitiveTopology(DisplayType);

			// ģ����ʼƫ��
			DesMeshHandle.Offset(i, DescriptorOffset);
			GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);

			// ��������ֻ�����ύ���ݵ�GPU������������Ⱦģ��

			// ����ͼԪ����ʵ��Ⱦģ�ͣ�
			GetD3dGraphicsCommandList()->DrawIndexedInstanced(
				renderingData.IndexSize,						// ��������
				1,												// ��ʾҪ���Ƶ�ʵ���������ֻ��Ҫ���Ƶ���ʵ��������Խ��˲�������Ϊ 1��
				renderingData.IndexOffsetPosition,				// ��ʾ���������������ĸ�λ�ÿ�ʼ��ȡ�������ݡ�ͨ������£����ֵΪ 0��
				renderingData.VertexOffsetPosition,				// ��ʾÿ�������ڶ��㻺�������������ʼλ�õ�ƫ��������������������������еģ�����Խ��˲�������Ϊ 0��
				0												// ��ʾ�ӵڼ���ʵ����ʼ���ơ����ֻ��Ҫ���Ƶ���ʵ��������Խ��˲�������Ϊ 0��
			);

		}
	}
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
	// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(static_cast<INT>(GetDrawMeshCount()), DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, DesHandle);		// ����b1�Ĵ��� (�ӿ��Ǻ���Ⱦ�����Է��ڼĴ���1�У�
}

void FGeometryMap::DrawTexture(float DeltaTime)
{
	// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(static_cast<INT>(GetDrawMeshCount() + GetDrawLightCount() + 1), DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(3, DesHandle);
}
