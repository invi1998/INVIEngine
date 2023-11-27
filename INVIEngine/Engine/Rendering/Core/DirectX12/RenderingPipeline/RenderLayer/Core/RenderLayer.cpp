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

	// 获取顶点/索引缓冲区视图
	D3D12_VERTEX_BUFFER_VIEW vbv = GeometryMap->Geometries[renderDate.GeometryKey].GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW ibv = GeometryMap->Geometries[renderDate.GeometryKey].GetIndexBufferView();


	// CD3DX12_GPU_DESCRIPTOR_HANDLE DesMeshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart());	// 模型描述handle

	D3D12_GPU_VIRTUAL_ADDRESS VirtualMeshAddress = GeometryMap->MeshConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();

	// 设置索引视图
	GetD3dGraphicsCommandList()->IASetIndexBuffer(&ibv);

	// 绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
	GetD3dGraphicsCommandList()->IASetVertexBuffers(
		0,		// 指定起始的顶点缓冲区槽号 `StartSlot`，这个值通常为 0 值范围 0-15
		1,		// 指定要绑定的顶点缓冲区数量 NumViews，这个值必须小于或等于硬件支持的最大顶点缓冲区数量
		&vbv	// 使用顶点缓冲区视图数组 pViews 来描述每个顶点缓冲区的位置、大小和布局等信息。
	);

	// 指定图元类型（点，线，面） 下面设置为 绘制由三角形构成的列表
	const D3D_PRIMITIVE_TOPOLOGY DisplayType = static_cast<D3D_PRIMITIVE_TOPOLOGY>((*renderDate.Mesh->GetMaterial())[0]->GetMaterialDisplayType());
	GetD3dGraphicsCommandList()->IASetPrimitiveTopology(DisplayType);

	// 模型起始偏移
	// DesMeshHandle.Offset(innerRenderData.MeshObjectIndex, DescriptorOffset);
	// GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);

	// 每个对象相对Geometry首地址的偏移
	D3D12_GPU_VIRTUAL_ADDRESS VAddress = VirtualMeshAddress + renderDate.MeshObjectIndex * MeshOffset;
	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(0, VAddress);

	// 上述步骤只是在提交数据到GPU，并不是在渲染模型

	// 绘制图元（真实渲染模型）
	GetD3dGraphicsCommandList()->DrawIndexedInstanced(
		renderDate.IndexSize,						// 顶点数量
		1,												// 表示要绘制的实例数。如果只需要绘制单个实例，则可以将此参数设置为 1。
		renderDate.IndexOffsetPosition,				// 表示从索引缓冲区中哪个位置开始读取索引数据。通常情况下，这个值为 0。
		renderDate.VertexOffsetPosition,				// 表示每个顶点在顶点缓冲区中相对于起始位置的偏移量。如果顶点数据是连续排列的，则可以将此参数设置为 0。
		0												// 表示从第几个实例开始绘制。如果只需要绘制单个实例，则可以将此参数设置为 0。
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
		// 更新模型
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
			RightVector.z,				UpVector.z,				ForwardVector.z * Scale.z,	0.f,
			Position.x,					Position.y,				Position.z,					1.f
		};

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

		GeometryMap->MeshConstantBufferViews.Update(renderingData.MeshObjectIndex, &OBJTransformation);
	}
}

void FRenderLayer::BuildPSO()
{
	BuildShader();

	// 构建PSO参数
	DirectXPipelineState->BuildParam();
}


