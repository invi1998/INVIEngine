#include "EngineMinimal.h"
#include "RenderLayer.h"

#include "Component/Mesh/Core/MeshComponent.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Material/Core/Material.h"
#include "Mesh/Core/ObjectTransformation.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"

FRenderLayer::FRenderLayer(): RenderPriority(0), RenderLayerType()
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
	DrawMesh(deltaTime);
}

void FRenderLayer::PostDraw(float deltaTime)
{
	// 清除渲染数据 std::vector<std::weak_ptr<FRenderingData>>::iterator
	for (auto iter = RenderData.begin(); iter != RenderData.end();)
	{
		if (iter->expired())
		{
			iter = RenderData.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void FRenderLayer::DrawObject(float deltaTime, std::weak_ptr<FRenderingData>& weakRenderDate, ERenderCondition rc)
{
	// 检查是否过期，判断弱指针是否被释放
	if (weakRenderDate.expired())
	{
		return;
	}

	if (std::shared_ptr<FRenderingData> renderDate = weakRenderDate.lock())	// 获取渲染数据
	{
		// 获取当前渲染对象的渲染条件
		auto GetRenderCondition = [&]() -> bool
			{
				if (!renderDate->Mesh->IsVisible()) return false;
				
				switch (rc)
				{
				case RC_Always:
					return true;
				case RC_Shadow:
					return renderDate->Mesh->IsCastShadow();
				default:
					return false;
				}
			};

		if (!GetRenderCondition())
		{
			// 不满足渲染条件，不渲染
			return;
		}

		UINT MeshOffset = GeometryMap->MeshConstantBufferViews.GetConstantBufferByteSize();

		// 获取顶点/索引缓冲区视图
		D3D12_VERTEX_BUFFER_VIEW vbv = GeometryMap->Geometries[renderDate->GeometryKey].GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW ibv = GeometryMap->Geometries[renderDate->GeometryKey].GetIndexBufferView();


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
		const D3D_PRIMITIVE_TOPOLOGY DisplayType = static_cast<D3D_PRIMITIVE_TOPOLOGY>((*renderDate->Mesh->GetMaterial())[0]->GetMaterialDisplayType());
		GetD3dGraphicsCommandList()->IASetPrimitiveTopology(DisplayType);

		// 模型起始偏移
		// DesMeshHandle.Offset(innerRenderData.MeshObjectIndex, DescriptorOffset);
		// GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);

		// 每个对象相对Geometry首地址的偏移
		D3D12_GPU_VIRTUAL_ADDRESS VAddress = VirtualMeshAddress + renderDate->MeshObjectIndex * MeshOffset;
		GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(0, VAddress);

		// 上述步骤只是在提交数据到GPU，并不是在渲染模型

		// 绘制图元（真实渲染模型）
		GetD3dGraphicsCommandList()->DrawIndexedInstanced(
			renderDate->IndexSize,						// 顶点数量
			1,												// 表示要绘制的实例数。如果只需要绘制单个实例，则可以将此参数设置为 1。
			renderDate->IndexOffsetPosition,				// 表示从索引缓冲区中哪个位置开始读取索引数据。通常情况下，这个值为 0。
			renderDate->VertexOffsetPosition,				// 表示每个顶点在顶点缓冲区中相对于起始位置的偏移量。如果顶点数据是连续排列的，则可以将此参数设置为 0。
			0												// 表示从第几个实例开始绘制。如果只需要绘制单个实例，则可以将此参数设置为 0。
		);
	}
}

void FRenderLayer::FindObjectDraw(float delta_time, const CMeshComponent* key)
{
	for (auto& weakRenderData : RenderData)
	{
		if (weakRenderData.expired())
		{
			continue;
		}

		if (std::shared_ptr<FRenderingData> renderData = weakRenderData.lock())
		{
			if (renderData->Mesh == key)
			{
				DrawObject(delta_time, weakRenderData);
				break;
			}
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
	for (auto& renderData : RenderData)
	{
		if (renderData.expired())
		{
			continue;
		}
		if (std::shared_ptr<FRenderingData> renderingData = renderData.lock())
		{
			// 更新模型
			XMFLOAT3& Position = renderingData->Mesh->GetPosition();
			XMFLOAT3& Scale = renderingData->Mesh->GetScale();

			// 拿到3个方向向量
			XMFLOAT3 RightVector = renderingData->Mesh->GetRightVector();
			XMFLOAT3 UpVector = renderingData->Mesh->GetUpVector();
			XMFLOAT3 ForwardVector = renderingData->Mesh->GetForwardVector();

			// 构造模型world
			/*renderingData->WorldMatrix = {
				RightVector.x * Scale.x,	UpVector.x,				ForwardVector.x,			0.f,
				RightVector.y,				UpVector.y * Scale.y,	ForwardVector.y,			0.f,
				RightVector.z,				UpVector.z,				ForwardVector.z * Scale.z,	0.f,
				Position.x,					Position.y,				Position.z,					1.f
			};*/

			EngineMath::BuildMatrix(renderingData->WorldMatrix, Position, Scale, RightVector, UpVector, ForwardVector);

			XMMATRIX ATRTIXMatrixWorld = XMLoadFloat4x4(&renderingData->WorldMatrix);
			XMMATRIX ATRTIXTextureWorld = XMLoadFloat4x4(&renderingData->TextureTransformationMatrix);

			// 更新法线矩阵
			XMVECTOR ATRTIXMatrixWorldDetemine = XMMatrixDeterminant(ATRTIXMatrixWorld);	// 先对世界矩阵求行列式
			XMMATRIX ATRTIXMatrixWorldInverse = XMMatrixInverse(&ATRTIXMatrixWorldDetemine, ATRTIXMatrixWorld);	// 再求逆矩阵

			FObjectTransformation OBJTransformation;
			// CPU端存储矩阵是先行后列的顺序，与GPU的默认情况（column_major）正好相反
			// 因此当我们要把CPU的矩阵通过Constant Buffer传递到GPU时，可以在存储矩阵时进行 矩阵的转置 操作
			XMStoreFloat4x4(&OBJTransformation.World, XMMatrixTranspose(ATRTIXMatrixWorld));
			XMStoreFloat4x4(&OBJTransformation.TextureTransformation, XMMatrixTranspose(ATRTIXTextureWorld));
			XMStoreFloat4x4(&OBJTransformation.NormalTransformation, ATRTIXMatrixWorldInverse);

			if (auto& material = (*renderingData->Mesh->GetMaterial())[0])
			{
				OBJTransformation.MaterialID = material->GetMaterialID();
				// ENGINE_LOG("材质id = %d, 材质名 = %s", OBJTransformation.MaterialID, material->GetBaseColorIndexKey().c_str());
			}

			GeometryMap->MeshConstantBufferViews.Update(renderingData->MeshObjectIndex, &OBJTransformation);
		}
		
	}
}

void FRenderLayer::BuildPSO()
{
	// 构建PSO参数
	DirectXPipelineState->BuildParam();

	// 先构建PSO参数，再构建Shader，不然的话我们后构建参数会导致Shader的参数不匹配
	BuildShader();

	// 需要额外定制的PSO可以走这里的代理构建
	if (BuildPsoDelegate.IsBound())
	{
		BuildPsoDelegate.Execute(DirectXPipelineState->GetGPSDesc());
	}
}

void FRenderLayer::ResetPSO()
{
}

void FRenderLayer::ResetPSO(EPipelineState ps)
{
}

void FRenderLayer::DrawMesh(float DeltaTime, ERenderCondition rc)
{
	for (auto& innerRenderData : RenderData)
	{
		DrawObject(DeltaTime, innerRenderData, rc);
	}
}

void FRenderLayer::Add(std::weak_ptr<FRenderingData>& weakRenderDate)
{
	RenderData.push_back(weakRenderDate);
}

void FRenderLayer::Remove(std::weak_ptr<FRenderingData>& weakRenderDate)
{
	if (weakRenderDate.expired()) return;

	for (auto iter = RenderData.begin(); iter != RenderData.end(); ++iter)
	{
		if (iter->expired())
		{
			continue;
		}
		if (std::shared_ptr<FRenderingData> renderDate = iter->lock())
		{
			if (renderDate == weakRenderDate.lock())
			{
				RenderData.erase(iter);
				break;
			}
		}
	}
}

void FRenderLayer::Remove(const size_t hashKey)
{
	for (auto iter = RenderData.begin(); iter != RenderData.end(); ++iter)
	{
		if (iter->expired())
		{
			continue;
		}
		if (std::shared_ptr<FRenderingData> renderDate = iter->lock())
		{
			if (renderDate->MeshHash == hashKey)
			{
				RenderData.erase(iter);
				break;
			}
		}
	}
}

void FRenderLayer::Clear()
{
	RenderData.clear();
}


