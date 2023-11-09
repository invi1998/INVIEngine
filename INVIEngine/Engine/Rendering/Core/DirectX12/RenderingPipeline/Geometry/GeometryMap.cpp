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

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));	// 创建一个二进制的缓冲区
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
	// +1 表示摄像机的常量缓冲区 (模型对象数量 + 灯光数量 + 摄像机 + 纹理贴图）
	DescriptorHeap.Build(GetDrawMeshCount() + GetDrawLightCount() + 1 + GetDrawTextureCount());
}

UINT FGeometryMap::GetDrawTextureCount()
{
	return RenderingTextureResourceViews->Size();
}

UINT FGeometryMap::GetDrawMeshCount()
{
	// 目前先写死成第一个
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
	// 创建常量缓冲区
	MeshConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshCount());

	// 描述堆句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	// 构建常量缓冲区
	MeshConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawMeshCount());
}

void FGeometryMap::BuildMaterialShaderResourceView()
{
	// 创建常量缓冲区
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
	//				//做ShaderIndex所有
	//				(*InMaterials)[j]->SetMaterialID(MaterialsSRV.size());

	//				MaterialsSRV.push_back((*InMaterials)[j]);
	//			}
	//		}
	//	}
	//}

	// 收集材质
	for (auto& geometry : Geometries)
	{
		for (auto& renderData: geometry.second.DescribeMeshRenderingData)
		{
			if (auto materials = renderData.Mesh->GetMaterial())
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
}

void FGeometryMap::BuildLightConstantBuffer()
{
	// 创建灯光常量缓冲区
	LightConstantBufferViews.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightCount());

	// 描述堆句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	// 构建常量缓冲区
	LightConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawLightCount(), GetDrawMeshCount());
}

void FGeometryMap::BuildViewportConstantBuffer()
{
	// 创建常量缓冲区
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

	// 描述堆句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	// 构建常量缓冲区
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
			// 单位化路径
			normalization_path(Paths.paths[i]);

			wchar_t TexturePath[1024] = { 0 };
			char_to_wchar_t(TexturePath, 1024, Paths.paths[i]);
			RenderingTextureResourceViews->LoadTextureResource(TexturePath);
		}
	}
}

void FGeometryMap::BuildTextureConstBuffer()
{
	// 偏移 = 模型渲染数 + 灯光渲染数 + 材质渲染数 + 视口
	RenderingTextureResourceViews->BuildTextureConstantBuffer(DescriptorHeap.GetHeap(), GetDrawMeshCount() + GetDrawLightCount() + 1);
}

void FGeometryMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	XMMATRIX ProjectionMatrix = XMLoadFloat4x4(&viewport_info.ProjectionMatrix);	// 投影矩阵
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&viewport_info.ViewMatrix);				// 视口矩阵
	for (auto& geometry : Geometries)
	{
		for (int i = 0; i < geometry.second.DescribeMeshRenderingData.size(); i++)
		{
			// 更新模型
			FRenderingData& renderingData = geometry.second.DescribeMeshRenderingData[i];
			{
				XMFLOAT3& Position = renderingData.Mesh->GetPosition();
				XMFLOAT3& Scale = renderingData.Mesh->GetScale();

				// 拿到3个方向向量
				XMFLOAT3 RightVector = renderingData.Mesh->GetRightVector();
				XMFLOAT3 UpVector = renderingData.Mesh->GetUpVector();
				XMFLOAT3 ForwardVector = renderingData.Mesh->GetForwardVector();

				// 构造模型world
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
			// CPU端存储矩阵是先行后列的顺序，与GPU的默认情况（column_major）正好相反
			// 因此当我们要把CPU的矩阵通过Constant Buffer传递到GPU时，可以在存储矩阵时进行 矩阵的转置 操作
			XMStoreFloat4x4(&OBJTransformation.World, XMMatrixTranspose(ATRTIXMatrixWorld));
			XMStoreFloat4x4(&OBJTransformation.TextureTransformation, XMMatrixTranspose(ATRTIXTextureWorld));

			if (auto& material = (*renderingData.Mesh->GetMaterial())[0])
			{
				OBJTransformation.MaterialID = material->GetMaterialID();
				// ENGINE_LOG("材质id = %d, 材质名 = %s", OBJTransformation.MaterialID, material->GetBaseColorIndexKey().c_str());
			}

			MeshConstantBufferViews.Update(i, &OBJTransformation);

		}
	}

	// 更新材质
	UpdateMaterialShaderResourceView(delta_time, viewport_info);

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

			if (auto basecolorPtr = RenderingTextureResourceViews->FindRenderingTexture(material->GetBaseColorIndexKey()))
			{
				MaterialConstantBuffer.BaseColorIndex = (*basecolorPtr)->RenderingTextureID;
				ENGINE_LOG("更新材质 材质索引 = %d, 材质名 = %ls", MaterialConstantBuffer.BaseColorIndex, (*basecolorPtr)->Name.c_str());
			}
			else
			{
				MaterialConstantBuffer.BaseColorIndex = -1;
			}

			// 法线贴图
			if (auto normalColor = RenderingTextureResourceViews->FindRenderingTexture(material->GetNormalIndexKey()))
			{
				MaterialConstantBuffer.NormalIndex = (*normalColor)->RenderingTextureID;
				ENGINE_LOG("更新法线贴图 材质索引 = %d, 材质名 = %ls", MaterialConstantBuffer.NormalIndex, (*normalColor)->Name.c_str());
			}
			else
			{
				MaterialConstantBuffer.NormalIndex = -1;
			}

			// 高光贴图
			if (auto specularColor = RenderingTextureResourceViews->FindRenderingTexture(material->GetSpecularIndexKey()))
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
	// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(static_cast<INT>(GetDrawMeshCount() + GetDrawLightCount()), DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);		// 更新b1寄存器 (视口是后渲染，所以放在寄存器1中）
}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (auto& geometry : Geometries | views::values)
	{
		// 获取顶点/索引缓冲区视图
		D3D12_VERTEX_BUFFER_VIEW vbv = geometry.GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW ibv = geometry.GetIndexBufferView();

		for (size_t i = 0; i < geometry.DescribeMeshRenderingData.size(); i++)
		{
			CD3DX12_GPU_DESCRIPTOR_HANDLE DesMeshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());	// 模型描述handle

			FRenderingData& renderingData = geometry.DescribeMeshRenderingData[i];

			// 设置索引视图
			GetD3dGraphicsCommandList()->IASetIndexBuffer(&ibv);

			// 绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
			GetD3dGraphicsCommandList()->IASetVertexBuffers(
				0,		// 指定起始的顶点缓冲区槽号 `StartSlot`，这个值通常为 0 值范围 0-15
				1,		// 指定要绑定的顶点缓冲区数量 NumViews，这个值必须小于或等于硬件支持的最大顶点缓冲区数量
				&vbv	// 使用顶点缓冲区视图数组 pViews 来描述每个顶点缓冲区的位置、大小和布局等信息。
			);

			// 指定图元类型（点，线，面） 下面设置为 绘制由三角形构成的列表
			D3D_PRIMITIVE_TOPOLOGY DisplayType = static_cast<D3D_PRIMITIVE_TOPOLOGY>((*renderingData.Mesh->GetMaterial())[0]->GetMaterialDisplayType());
			GetD3dGraphicsCommandList()->IASetPrimitiveTopology(DisplayType);

			// 模型起始偏移
			DesMeshHandle.Offset(i, DescriptorOffset);
			GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);

			// 上述步骤只是在提交数据到GPU，并不是在渲染模型

			// 绘制图元（真实渲染模型）
			GetD3dGraphicsCommandList()->DrawIndexedInstanced(
				renderingData.IndexSize,						// 顶点数量
				1,												// 表示要绘制的实例数。如果只需要绘制单个实例，则可以将此参数设置为 1。
				renderingData.IndexOffsetPosition,				// 表示从索引缓冲区中哪个位置开始读取索引数据。通常情况下，这个值为 0。
				renderingData.VertexOffsetPosition,				// 表示每个顶点在顶点缓冲区中相对于起始位置的偏移量。如果顶点数据是连续排列的，则可以将此参数设置为 0。
				0												// 表示从第几个实例开始绘制。如果只需要绘制单个实例，则可以将此参数设置为 0。
			);

		}
	}
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
	// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(static_cast<INT>(GetDrawMeshCount()), DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, DesHandle);		// 更新b1寄存器 (视口是后渲染，所以放在寄存器1中）
}

void FGeometryMap::DrawTexture(float DeltaTime)
{
	// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(static_cast<INT>(GetDrawMeshCount() + GetDrawLightCount() + 1), DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(3, DesHandle);
}
