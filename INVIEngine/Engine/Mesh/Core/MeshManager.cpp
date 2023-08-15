#include "MeshManager.h"

#include "ObjectTransformation.h"
#include "Config/EngineRenderConfig.h"
#include "Core/Viewport/ViewportTransformation.h"
#include "Mesh/BoxMesh.h"
#include "Mesh/CCylinderMesh.h"
#include "Mesh/ConeMesh.h"
#include "Mesh/CPlaneMesh.h"
#include "Mesh/CustomMesh.h"
#include "Mesh/SphereMesh.h"
#include "Rendering/Core/Buffer/ConstructBuffer.h"
#include "Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"

CMeshManager::CMeshManager()
	: IndexSize(0), VertexSizeInBytes(0), VertexStrideInBytes(0), IndexSizeInBytes(0), IndexFormat(DXGI_FORMAT_R16_UINT),
	WorldMatrix(EngineMath::IdentityMatrix4x4())
{
}

CMeshManager::~CMeshManager()
= default;

void CMeshManager::BeginInit()
{
	CCoreMinimalObject::BeginInit();
}

void CMeshManager::Tick(float DeltaTime)
{
	CCoreMinimalObject::Tick(DeltaTime);
}

void CMeshManager::Init()
{
	IRenderingInterface::Init();
}

void CMeshManager::PreDraw(float DeltaTime)
{
	IRenderingInterface::PreDraw(DeltaTime);

	// 重置命令列表，因为我们每一帧都会有新的提交列表
	GetD3dGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());
}

void CMeshManager::Draw(float DeltaTime)
{
	IRenderingInterface::Draw(DeltaTime);

	ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
	GetD3dGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);

	GetD3dGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());

	// 将我们的顶点缓冲区和渲染流水线绑定

	// 获取顶点缓冲区视图
	D3D12_VERTEX_BUFFER_VIEW vbv = GetVertexBufferView();
	// 绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
	GetD3dGraphicsCommandList()->IASetVertexBuffers(
		0,		// 指定起始的顶点缓冲区槽号 `StartSlot`，这个值通常为 0 值范围 0-15
		1,		// 指定要绑定的顶点缓冲区数量 NumViews，这个值必须小于或等于硬件支持的最大顶点缓冲区数量
		&vbv	// 使用顶点缓冲区视图数组 pViews 来描述每个顶点缓冲区的位置、大小和布局等信息。
	);

	D3D12_INDEX_BUFFER_VIEW ibv = GetIndexBufferView();
	GetD3dGraphicsCommandList()->IASetIndexBuffer(
		&ibv	// 索引缓冲区视图
	);

	// 上述步骤只是在提交数据到GPU，并不是在渲染模型

	// 指定图元类型（点，线，面） 下面设置为 绘制由三角形构成的列表
	GetD3dGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 计算第二个描述符在描述符堆中应该存放位置的偏移
	int HeapOffset = 1;
	// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CBVHeap->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(0, DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesHandle);		// 更新b0寄存器

	DesHandle.Offset(1, DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);		// 更新b1寄存器

	// 绘制图元（真实渲染模型）
	GetD3dGraphicsCommandList()->DrawIndexedInstanced(
		IndexSize,		// 顶点数量
		1,				// 表示要绘制的实例数。如果只需要绘制单个实例，则可以将此参数设置为 1。
		0,				// 表示从索引缓冲区中哪个位置开始读取索引数据。通常情况下，这个值为 0。
		0,				// 表示每个顶点在顶点缓冲区中相对于起始位置的偏移量。如果顶点数据是连续排列的，则可以将此参数设置为 0。
		0				// 表示从第几个实例开始绘制。如果只需要绘制单个实例，则可以将此参数设置为 0。
	);
}

void CMeshManager::PostDraw(float DeltaTime)
{
	IRenderingInterface::PostDraw(DeltaTime);
}

void CMeshManager::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 常量缓冲区构建

	// 堆描述
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
	HeapDesc.NumDescriptors = 2;		// 描述数量 2
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;	// 指定堆类型，常量缓冲区视图，着色器资源视图，无序访问视图组合类型描述符
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// 指定标记，着色器可见
	HeapDesc.NodeMask = 0;
	// 创建描述符堆
	GetD3dDevice()->CreateDescriptorHeap(
		&HeapDesc,
		IID_PPV_ARGS(&CBVHeap)
	);

	// 常量缓冲区1 object
	{
		// 构建常量缓冲区描述符堆 CBVHeap

		ObjectConstants = std::make_shared<FRenderingResourcesUpdate>();
		ObjectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
		D3D12_GPU_VIRTUAL_ADDRESS ObAddr = ObjectConstants.get()->GetBuffer()->GetGPUVirtualAddress();

		// 常量缓冲区描述
		D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDes;
		CBVDes.BufferLocation = ObAddr;		// 常量缓冲区地址
		CBVDes.SizeInBytes = ObjectConstants->GetConstantsBufferByteSize();		// 获取常量缓冲区字节大小

		GetD3dDevice()->CreateConstantBufferView(&CBVDes, CBVHeap->GetCPUDescriptorHandleForHeapStart());
	}

	// 常量缓冲区2 viewport
	{
		// 视口常量缓冲区
		ViewportConstants = std::make_shared<FRenderingResourcesUpdate>();
		ViewportConstants->Init(GetD3dDevice().Get(), sizeof(FViewportTransformation), 1);
		D3D12_GPU_VIRTUAL_ADDRESS ViewportAddr = ViewportConstants.get()->GetBuffer()->GetGPUVirtualAddress();

		// 计算第二个描述符在描述符堆中应该存放位置的偏移
		int HeapOffset = 1;
		// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
		UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CBVHeap->GetCPUDescriptorHandleForHeapStart());
		DesHandle.Offset(HeapOffset, DescriptorOffset);

		// 常量缓冲区描述
		D3D12_CONSTANT_BUFFER_VIEW_DESC viewportCBVDes;
		viewportCBVDes.BufferLocation = ViewportAddr;		// 常量缓冲区地址
		viewportCBVDes.SizeInBytes = ViewportConstants->GetConstantsBufferByteSize();		// 获取常量缓冲区字节大小
		// 传入我们计算好偏移值的handle
		GetD3dDevice()->CreateConstantBufferView(&viewportCBVDes, DesHandle);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///构建根签名

	// 渲染对象（物体） CBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjectCBV;	// 常量缓冲区区描述符范围 描述符范围（Descriptor Range）的创建
	DescriptorRangeObjectCBV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV,	// 指定视图（这里指向常量缓冲区视图 （描述符类型））
		1,									// 描述数量 1
		0);						// 基于那个着色器的寄存器（绑定寄存器（shaderRegister 和 registerSpace））

	// 视口 CBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;	// 常量缓冲区区描述符范围 描述符范围（Descriptor Range）的创建
	DescriptorRangeViewportCBV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV,	// 指定视图（这里指向常量缓冲区视图 （描述符类型））
		1,									// 描述数量 1
		1);						// 基于那个着色器的寄存器（绑定寄存器（shaderRegister 和 registerSpace））

	// 创建根参数，使用上面的描述符范围
	CD3DX12_ROOT_PARAMETER RootParam[2];
	RootParam[0].InitAsDescriptorTable(
		1,							// 描述符数量
		&DescriptorRangeObjectCBV		// 指向描述符范围数组的指针
		// D3D12_SHADER_VISIBILITY_ALL	// 着色器可见性(该值默认为shader可见，一般不用设置）
	);

	RootParam[1].InitAsDescriptorTable(
		1,							// 描述符数量
		&DescriptorRangeViewportCBV		// 指向描述符范围数组的指针
		// D3D12_SHADER_VISIBILITY_ALL	// 着色器可见性(该值默认为shader可见，一般不用设置）
	);

	// 序列化根签名，将我们当前的描述二进制连续的一个内存(将根签名（Root Signature）序列化为字节流数据)

	// 根签名（Root Signature）描述结构体的创建
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		2,			// 参数数量
		RootParam,	// 根签名参数
		0,			// 静态采样数量
		nullptr,	// 静态采样数据
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT // 指定根签名布局选项 表示根签名允许输入汇编程序访问根常量数据。
	);

	// 这个函数可以将根签名序列化为一个 ID3DBlob 对象，该对象包含了一个字节流，可以用于在后续的操作中传递、保存和加载根签名。
	ComPtr<ID3DBlob> SerializeRootSignatureBlob;	// 存储序列化成功的二进制数据流
	ComPtr<ID3DBlob> ErrorBlob;						// 存储序列化失败的信息
	D3D12SerializeRootSignature(
		&RootSignatureDesc,							// 传入要序列化的根签名描述结构体指针
		D3D_ROOT_SIGNATURE_VERSION_1,				// 根签名的版本号
		SerializeRootSignatureBlob.GetAddressOf(),	// 序列化后的根签名数据
		ErrorBlob.GetAddressOf()					// 该对象包含了在序列化过程中出现错误时产生的错误信息
	);

	if (ErrorBlob)
	{
		ENGINE_LOG_ERROR("%s", static_cast<char*>(ErrorBlob->GetBufferPointer()));
	}

	// 成功，创建根签名
	GetD3dDevice()->CreateRootSignature(
		0,			// 表示要创建的根签名对象所属的节点掩码，通常设置为 0, 表示单个CPU。(设备的物理适配器）
		SerializeRootSignatureBlob->GetBufferPointer(),			// 表示包含序列化根签名数据的内存地址。
		SerializeRootSignatureBlob->GetBufferSize(),			// 表示序列化根签名数据的字节长度。
		IID_PPV_ARGS(&RootSignature)							// 传入根签名接收地址
	);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///构建shader HLSL
	VertexShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "VSMain", "vs_5_0");
	PixelShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "PSMain", "ps_5_0");

	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	// 颜色，这里这个偏移是12字节，因为我们上面位置是3个元素，每个元素是4字节（32位），所以到了颜色这里就是 3*4 = 12字节的偏移了
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 构建模型

	IndexSize = InRenderingData->IndexData.size();
	VertexStrideInBytes = sizeof(FVertex);

	// 计算顶点数据字节大小
	VertexSizeInBytes = InRenderingData->VertexData.size() * sizeof(FVertex);
	IndexSizeInBytes = InRenderingData->IndexData.size() * sizeof(uint16_t);

	// 创建缓冲区
	ANALYSIS_RESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));	// 创建一个二进制的缓冲区
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), VertexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData->IndexData.data(), IndexSizeInBytes);

	ConstructBuffer::FConstructDefaultBuffer ConstructDefaultBuffer;

	GPUVertexBufferPtr = ConstructDefaultBuffer.ConstructDefaultBuffer(TempVertexBufferPtr, InRenderingData->VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer.ConstructDefaultBuffer(TempIndexBufferPtr, InRenderingData->IndexData.data(), IndexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));	// 创建一个二进制的缓冲区

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///PSO渲染流水线绑定

	// 描述当前渲染管线状态
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc{};

	// 绑定输入布局
	GPSDesc.InputLayout.NumElements = static_cast<UINT>(InputElementDesc.size());
	GPSDesc.InputLayout.pInputElementDescs = InputElementDesc.data();

	// 绑定根签名
	GPSDesc.pRootSignature = RootSignature.Get();        // 根签名

	// 绑定着色器
	GPSDesc.VS.pShaderBytecode = static_cast<BYTE*>(VertexShader.GetBufferPointer());                   // 顶点着色器
	GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
	GPSDesc.PS.pShaderBytecode = PixelShader.GetBufferPointer();                   // 像素着色器
	GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();

	// 配置光栅化状态
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);		// 光栅化状态
	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;			// 已线框方式显示

	GPSDesc.SampleMask = UINT_MAX;		// 多重采样遮罩（混合状态下的实例蒙版）多重采样下，最多是可以采32个的样板（000...000),每个0表示一个样板位，有32个，你想采哪个就设置哪位为1，UINT_MAX表示0xffffffff采样全部的样板

	// 指定图元拓扑类型
	GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;		// 三角形

	// 指定渲染目标
	GPSDesc.NumRenderTargets = 1;

	// 指定混合状态
	GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);		// 默认混合

	// 启用深度模板
	GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	// 默认深度模板

	// 设置采样
	GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetDXGISampleCount();		// 采样数量
	GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetDXGISampleQuality();	// 采样质量
	GPSDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat();			// 渲染目标视图格式（后台缓冲区格式）
	GPSDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilFormat();			// 深度模板缓冲区格式

	// 通过D3D创建渲染管线状态对象
	ANALYSIS_RESULT(GetD3dDevice()->CreateGraphicsPipelineState(
		&GPSDesc,
		IID_PPV_ARGS(&PSO)
	));
}

void CMeshManager::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	/*XMUINT3 MeshPosition = XMUINT3(5.f, 5.f, 5.f);

	XMVECTOR Pos = XMVectorSet(MeshPosition.x, MeshPosition.y, MeshPosition.z, 1.f);
	XMVECTOR ViewTarget = XMVectorZero();
	XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

	XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);

	XMStoreFloat4x4(const_cast<XMFLOAT4X4*>(&viewport_info.ViewMatrix), ViewLookAt);*/

	// 更新视口
	XMMATRIX ProjectionMatrix = XMLoadFloat4x4(&viewport_info.ProjectionMatrix);	// 投影矩阵
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&viewport_info.ViewMatrix);				// 视口矩阵

	XMMATRIX MatrixWorld = XMLoadFloat4x4(&WorldMatrix);

	FObjectTransformation OBJTransformation;
	XMStoreFloat4x4(&OBJTransformation.World, XMMatrixTranspose(MatrixWorld));

	ObjectConstants->Update(0, &OBJTransformation);


	XMMATRIX ViewProjection= XMMatrixMultiply(ViewMatrix, ProjectionMatrix);

	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProjection));	// 存储之前记得对矩阵进行转置

	ViewportConstants->Update(0, &ViewportTransformation);

}

D3D12_VERTEX_BUFFER_VIEW CMeshManager::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW vbv;

	vbv.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	vbv.SizeInBytes = VertexSizeInBytes;
	vbv.StrideInBytes = VertexStrideInBytes;

	return vbv;
}

D3D12_INDEX_BUFFER_VIEW CMeshManager::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW ibv;

	ibv.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
	ibv.SizeInBytes = IndexSizeInBytes;
	ibv.Format = IndexFormat;

	return ibv;
}

CMesh* CMeshManager::CreateBoxMesh(float InHeight, float InWidth, float InDepth)
{
	return CreateMesh<CBoxMesh>(InHeight, InWidth, InDepth);
}

CMesh* CMeshManager::CreateConeMesh(float InRadius, float InHeight, uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	return CreateMesh<CConeMesh>(InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManager::CreateCylinderMesh(float InTopRadius, float InBottomRadius, float InHeight,
	uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return CreateMesh<CCylinderMesh>(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManager::CreatePlaneMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide,
	uint32_t InWidthSubdivide)
{
	return CreateMesh<CPlaneMesh>(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

CMesh* CMeshManager::CreateSphereMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return CreateMesh<CSphereMesh>(InRadius, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManager::CreateMesh(const string& InPath)
{
	return CreateMesh<CCustomMesh>(InPath);
}

template <typename T, typename ... ParamTypes>
T* CMeshManager::CreateMesh(ParamTypes&&... Params)
{
	T* MyMesh = new T();

	// 提取模型资源
	FMeshRenderingData MeshData;
	MyMesh->CreateMesh(MeshData, std::forward<ParamTypes>(Params)...);

	MyMesh->BeginInit();

	// 构建mesh
	// BuildMesh(&MeshData);
	RenderingPipeline.BuildMesh(MyMesh, &MeshData);

	MyMesh->Init();

	return MyMesh;
}
