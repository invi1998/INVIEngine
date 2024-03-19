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

// 声明静态变量
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
	// 判断当前模型是否已经被添加过了
	if (!bRenderingDataExistence(inMesh))
	{
		if (std::shared_ptr<FRenderLayer> renderLayer = FRenderLayerManage::FindByRenderLayer(inMesh->GetRenderLayerType()))
		{
			RenderingDataPool.insert(pair<size_t, std::shared_ptr<FRenderingData>>(meshHash, std::make_shared<FRenderingData>()));

			RenderingDataPoolVector.push_back(std::make_shared<FRenderingData>());

			std::shared_ptr<FRenderingData> InRenderingData = RenderingDataPoolVector[RenderingDataPoolVector.size() - 1];

			renderLayer->RenderData.push_back(InRenderingData);

			// 求出模型的AABB包围盒
			XMFLOAT3 Min = { FLT_MAX, FLT_MAX, FLT_MAX };			// 最小点
			XMFLOAT3 Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };		// 最大点

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
			// 记录顶点数据
			InRenderingData->IndexSize = MeshData.IndexData.size();
			InRenderingData->VertexSize = MeshData.VertexData.size();
			// 记录数据偏移
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

			// 一种高效的数据插入方式
			// 索引合并
			MeshRenderingData.IndexData.insert(MeshRenderingData.IndexData.end(), MeshData.IndexData.begin(), MeshData.IndexData.end());
			// 顶点合并
			MeshRenderingData.VertexData.insert(MeshRenderingData.VertexData.end(), MeshData.VertexData.begin(), MeshData.VertexData.end());
		}
	}
}

void FGeometry::Build()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 构建模型

	const UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	const UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();

	// 创建缓冲区
	ANALYSIS_RESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));	// 创建一个二进制的缓冲区
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
	// 判断当前模型是否已经被添加过了
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
	// +1 表示摄像机的常量缓冲区 (模型对象数量 + 灯光数量 + 摄像机 + 纹理贴图 + CubeMap + ShadowMap）
	// DescriptorHeap.Build(GetDrawMeshCount() + GetDrawLightCount() + 1 + GetDrawTexture2DCount() + GetDrawCubeMapCount());

	// 纹理贴图 + cube map(静态cubeMap) + CubeMapShadow + ui, 只需要为texture2D图片分配堆内存，因为我们将其他的(模型对象数量 + 灯光数量 + 摄像机)从常量缓冲区分离出来了，只有纹理和cubemap还在继续使用描述表
	DescriptorHeap.Build(GetDrawTexture2DCount() + // 纹理贴图数量
		GetDrawCubeMapCount() +	// CubeMap数量
		1 + // 反射Cubemap 动态反射
		1 +	// 阴影贴图 直射灯，聚光灯
		1 + // shadowCubeMap 6个面 (点光源阴影）
		1 + // UI
		1 +	// 法线
		1 + // 深度
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
	// 目前先写死成第一个
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
	// 创建常量缓冲区
	MeshConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshCount());

	//// 描述堆句柄
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//// 构建常量缓冲区
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
				// 设置材质ID
				(*materials)[i]->SetMaterialID(MaterialsSRV.size());
				// 保存材质
				MaterialsSRV.push_back((*materials)[i]);
			}
		}
		
	}

	// 创建常量缓冲区
	MaterialConstantBufferViews.CreateConstant(sizeof(FMaterialConstantBuffer), GetDrawMaterialCount(), false);
}

void FGeometryMap::BuildLightConstantBuffer()
{
	// 创建灯光常量缓冲区
	LightConstantBufferViews.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightCount());

	//// 描述堆句柄
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//// 构建常量缓冲区
	//LightConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawLightCount(), GetDrawMeshCount());
}

void FGeometryMap::BuildFogConstantBuffer()
{
	// 构建雾的常量缓冲区
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
	// 创建常量缓冲区 (主视口 + 动态反射视口 + shadow摄像机视口 + 点光源CubeMapShadow视口）
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1 + GetDynamicReflectionViewportNum() + 1 + 6 + viewPortOffset);

	//// 描述堆句柄
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//// 构建常量缓冲区
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
			// 单位化路径
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
				// 2D纹理
				RenderingTexture2DResources->LoadTextureResource(TexturePath);
			}
			
		}
	}
}

void FGeometryMap::BuildTextureConstBuffer()
{
	// 偏移 = 模型渲染数 + 灯光渲染数 + 材质渲染数 + 视口
	// RenderingTexture2DResources->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), GetDrawMeshCount() + GetDrawLightCount() + 1);

	// 因为将模型，灯光，视口都从常量缓冲区分离出来了，所以这里偏移直接给0就行
	RenderingTexture2DResources->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), 0);

	// 构建CubeMap
	// 偏移 = 模型渲染数 + 灯光渲染数 + 材质渲染数 + 视口 + 贴图数量
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
	XMMATRIX ProjectionMatrix = XMLoadFloat4x4(&viewport_info.ProjectionMatrix);	// 投影矩阵
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&viewport_info.ViewMatrix);				// 视口矩阵

	// 更新视口
	XMMATRIX ViewProjection = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);
	FViewportTransformation ViewportTransformation;
	ViewportTransformation.CameraPosition = viewport_info.CameraPosition;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProjection));	// 存储之前记得对矩阵进行转置

	ViewportConstantBufferViews.Update(constBufferOffset, &ViewportTransformation);
}

void FGeometryMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{

	// 更新材质
	UpdateMaterialShaderResourceView(delta_time, viewport_info);

	// 更新灯光
	UpdateLight(delta_time, viewport_info);

	// 更新视口
	UpdateCalculationViewport(viewport_info, 0);

	// 更新雾
	UpdateFog(delta_time, viewport_info);

	// 更新阴影
	UpdateShadow(delta_time, viewport_info);
	
}

void FGeometryMap::UpdateMaterialShaderResourceView(float delta_time, const FViewportInfo& viewport_info)
{
	FMaterialConstantBuffer MaterialConstantBuffer = {};
	// 更新材质
	for (auto& material : MaterialsSRV)
	{
		if (material->IsDirty())
		{
			MaterialConstantBuffer.MaterialType = material->GetMaterialType();	// 材质类型
			MaterialConstantBuffer.BaseColor = material->GetBaseColor();	// 基础颜色
			MaterialConstantBuffer.Roughness = material->GetRoughness();	// 粗糙度
			MaterialConstantBuffer.SpecularColor = material->GetSpecularColor();	// 高光颜色
			MaterialConstantBuffer.FresnelF0 = material->GetFresnelF0();		// 菲涅尔F0
			MaterialConstantBuffer.Transparency = material->GetTransparency();		// 透明度
			MaterialConstantBuffer.RefractiveValue = material->GetRefractiveValue();	// 折射率
			MaterialConstantBuffer.Metallicity = material->GetMetallicity();		// 金属度

			if (auto basecolorPtr = FindRenderingTexture(material->GetBaseColorIndexKey()))
			{
				MaterialConstantBuffer.BaseColorIndex = (*basecolorPtr)->RenderingTextureID;
				ENGINE_LOG("更新材质 材质索引 = %d, 材质名 = %ls", MaterialConstantBuffer.BaseColorIndex, (*basecolorPtr)->Name.c_str());
			}
			else
			{
				MaterialConstantBuffer.BaseColorIndex = -1;
			}

			// 法线贴图
			if (auto normalColor = FindRenderingTexture(material->GetNormalIndexKey()))
			{
				MaterialConstantBuffer.NormalIndex = (*normalColor)->RenderingTextureID;
				ENGINE_LOG("更新法线贴图 材质索引 = %d, 材质名 = %ls", MaterialConstantBuffer.NormalIndex, (*normalColor)->Name.c_str());
			}
			else
			{
				MaterialConstantBuffer.NormalIndex = -1;
			}

			// 高光贴图
			if (auto specularColor = FindRenderingTexture(material->GetSpecularIndexKey()))
			{
				MaterialConstantBuffer.SpecularIndex = (*specularColor)->RenderingTextureID;
				ENGINE_LOG("更新高光贴图 材质索引 = %d, 材质名 = %ls", MaterialConstantBuffer.SpecularIndex, (*specularColor)->Name.c_str());
			}
			else
			{
				MaterialConstantBuffer.SpecularIndex = -1;
			}

			XMFLOAT4X4 MaterialTransform = material->GetTransformation();
			XMMATRIX Transform = XMLoadFloat4x4(&MaterialTransform);
			// 将材质里的行矩阵转为列矩阵传入shader中
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
	// 更新灯光
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
					// 平行光 这里构建的是正交矩阵

					DynamicShadowMap.BuildOrthoProjectionMatrix({ 0, 0, 0 },lightComponent->GetForwardVector(), 70.f);

					// 获取ViewMatrix
					XMFLOAT4X4 ViewMatrix = DynamicShadowMap.GetViewMatrix();

					// 获取ProjectionMatrix
					XMFLOAT4X4 ProjectionMatrix = DynamicShadowMap.GetProjectionMatrix();

					XMMATRIX ShadowViewMatrix = XMLoadFloat4x4(&ViewMatrix);
					XMMATRIX ShadowProjectionMatrix = XMLoadFloat4x4(&ProjectionMatrix);

					// 准备一个变换矩阵 该矩阵可以将我们NDC空间里的[-1, 1]转换到[0, 1]
					XMMATRIX Transform = {
						0.5f, 0.0f, 0.0f, 0.0f,
						0.0f, -0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.5f, 0.5f, 0.0f, 1.0f
					};

					// NDC [-1, 1] ==> [0, 1]
					XMMATRIX ShadowViewProjectionMatrix = ShadowViewMatrix * ShadowProjectionMatrix * Transform;

					// 存储Shadow变化矩阵 (记得要先对矩阵进行转置)
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

						// 聚光灯 这里构建的是透视投影矩阵

						DynamicShadowMap.BuildPerspectiveProjectionMatrix(lightComponent->GetPosition(), lightComponent->GetForwardVector(), 70.f);

						// 获取ViewMatrix
						XMFLOAT4X4 ViewMatrix = DynamicShadowMap.GetViewMatrix();

						// 获取ProjectionMatrix
						XMFLOAT4X4 ProjectionMatrix = DynamicShadowMap.GetProjectionMatrix();

						XMMATRIX ShadowViewMatrix = XMLoadFloat4x4(&ViewMatrix);
						XMMATRIX ShadowProjectionMatrix = XMLoadFloat4x4(&ProjectionMatrix);

						// 准备一个变换矩阵 该矩阵可以将我们NDC空间里的[-1, 1]转换到[0, 1]
						XMMATRIX Transform = {
							0.5f, 0.0f, 0.0f, 0.0f,
							0.0f, -0.5f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.5f, 0.5f, 0.0f, 1.0f
						};

						// NDC [-1, 1] ==> [0, 1]
						XMMATRIX ShadowViewProjectionMatrix = ShadowViewMatrix * ShadowProjectionMatrix * Transform;

						// 存储Shadow变化矩阵 (记得要先对矩阵进行转置)
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
	// 更新阴影
	DynamicShadowMap.UpdateCalculations(delta_time, viewport_info);
}

void FGeometryMap::UpdateFog(float delta_time, const FViewportInfo& viewport_info)
{
	// 更新雾
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
	// 渲染灯光
	DrawLight(DeltaTime);

	// 渲染贴图
	DrawTexture2D(DeltaTime);

	// 渲染CubeMap (立方体贴图）
	DrawCubeMapTexture(DeltaTime);

	// 渲染材质
	DrawMaterial(DeltaTime);

	// 渲染雾
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
	//// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
	//UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	//DesHandle.Offset(static_cast<INT>(GetDrawMeshCount() + GetDrawLightCount()), DescriptorOffset);
	//GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);		// 更新b1寄存器 (视口是后渲染，所以放在寄存器1中）


	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());

}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	// GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(2, LightConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawMaterial(float DeltaTime)
{

	GetD3dGraphicsCommandList()->SetGraphicsRootShaderResourceView(
		4,	// 要绑定的根签名槽的索引
		MaterialConstantBufferViews.GetBuffer()->GetGPUVirtualAddress()	// 着色器资源视图的 GPU 描述符句柄。
		);
}

void FGeometryMap::DrawLight(float DeltaTime)
{
	//// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
	//UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	//DesHandle.Offset(static_cast<INT>(GetDrawMeshCount()), DescriptorOffset);
	//GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, DesHandle);		// 更新b1寄存器 (视口是后渲染，所以放在寄存器1中）

	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(2, LightConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawShadow(float DeltaTime)
{
	DynamicShadowMap.Draw(DeltaTime);
}

void FGeometryMap::DrawTexture2D(float DeltaTime)
{
	// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(0, DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(5, DesHandle);

}

void FGeometryMap::DrawCubeMapTexture(float DeltaTime)
{
	// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
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
