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
	// 判断当前模型是否已经被添加过了
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
			// 记录顶点数据
			InRenderingData.IndexSize = MeshData.IndexData.size();
			InRenderingData.VertexSize = MeshData.VertexData.size();
			// 记录数据偏移
			InRenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();
			InRenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();

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

void FGeometry::DuplicateMesh(CMeshComponent* mesh_component, const FRenderingData& rendering_data, int geometryKey)
{
	// 判断当前模型是否已经被添加过了
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

	// 全量遍历查找渲染层级里的渲染数据
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
		// 传入指定的渲染层级进行查找
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
	// +1 表示摄像机的常量缓冲区 (模型对象数量 + 灯光数量 + 摄像机 + 纹理贴图 + CubeMap）
	// DescriptorHeap.Build(GetDrawMeshCount() + GetDrawLightCount() + 1 + GetDrawTexture2DCount() + GetDrawCubeMapCount());

	// 纹理贴图 + cube map, 只需要为texture2D图片分配堆内存，因为我们将其他的(模型对象数量 + 灯光数量 + 摄像机)从常量缓冲区分离出来了，只有纹理和cubemap还在继续使用描述表
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
	// 收集材质
	for (const auto& layer : FRenderLayerManage::RenderLayers)
	{
		for (const auto& renderData : layer->RenderData)
		{
			if (const auto materials = renderData.Mesh->GetMaterial())
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

bool FGeometryMap::IsStartUpFog()
{
	return Fog != nullptr;
}

void FGeometryMap::BuildViewportConstantBuffer()
{
	// 创建常量缓冲区
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

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
				RenderingCubeMapResourceViews->LoadTextureResource(TexturePath);
			}
			else
			{
				// 2D纹理
				RenderingTexture2DResourceViews->LoadTextureResource(TexturePath);
			}
			
		}
	}
}

void FGeometryMap::BuildTextureConstBuffer()
{
	// 偏移 = 模型渲染数 + 灯光渲染数 + 材质渲染数 + 视口
	// RenderingTexture2DResourceViews->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), GetDrawMeshCount() + GetDrawLightCount() + 1);

	// 因为将模型，灯光，视口都从常量缓冲区分离出来了，所以这里偏移直接给0就行
	RenderingTexture2DResourceViews->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), 0);

	// 构建CubeMap
	// 偏移 = 模型渲染数 + 灯光渲染数 + 材质渲染数 + 视口 + 贴图数量
	RenderingCubeMapResourceViews->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), GetDrawTexture2DCount());
}

void FGeometryMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{

	// 更新材质
	UpdateMaterialShaderResourceView(delta_time, viewport_info);

	XMMATRIX ProjectionMatrix = XMLoadFloat4x4(&viewport_info.ProjectionMatrix);	// 投影矩阵
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&viewport_info.ViewMatrix);				// 视口矩阵
	

	

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

	// 更新视口
	XMMATRIX ViewProjection = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);
	FViewportTransformation ViewportTransformation;
	ViewportTransformation.CameraPosition = viewport_info.CameraPosition;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProjection));	// 存储之前记得对矩阵进行转置

	ViewportConstantBufferViews.Update(0, &ViewportTransformation);

	// 更新雾
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
	// 更新材质
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
	// 渲染视口
	DrawViewport(DeltaTime);

	// 渲染灯光
	DrawLight(DeltaTime);

	// 渲染贴图
	DrawTexture(DeltaTime);

	// 渲染材质
	DrawMaterial(DeltaTime);

	// 渲染模型
	// DrawMesh(DeltaTime);

	// 渲染雾
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

void FGeometryMap::DrawTexture(float DeltaTime)
{
	// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
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
