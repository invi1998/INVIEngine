#include "Mesh.h"
#include "EngineMinimal.h"
#include "Platform/Windows/WindowsEngine.h"

FObjectTransformation::FObjectTransformation()
	: World(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	)
{
}

/**
 * \brief /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */
FMesh::FMesh()
	: IndexSize(0), VertexSizeInBytes(0), VertexStrideInBytes(0), IndexSizeInBytes(0), IndexFormat(DXGI_FORMAT_R16_UINT)
{
}

FMesh::~FMesh()
{
}

void FMesh::Init()
{
	IRenderingInterface::Init();
}

void FMesh::Draw(float DeltaTime)
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

	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

	// 绘制图元（真实渲染模型）
	GetD3dGraphicsCommandList()->DrawIndexedInstanced(
		IndexSize,		// 顶点数量
		1,				// 表示要绘制的实例数。如果只需要绘制单个实例，则可以将此参数设置为 1。
		0,				// 表示从索引缓冲区中哪个位置开始读取索引数据。通常情况下，这个值为 0。
		0,				// 表示每个顶点在顶点缓冲区中相对于起始位置的偏移量。如果顶点数据是连续排列的，则可以将此参数设置为 0。
		0				// 表示从第几个实例开始绘制。如果只需要绘制单个实例，则可以将此参数设置为 0。
	);
}

void FMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 常量缓冲区构建

	// 构建常量缓冲区描述符堆 CBVHeap

	// 堆描述
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
	HeapDesc.NumDescriptors = 1;		// 描述数量 1
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;	// 指定堆类型，常量缓冲区视图，着色器资源视图，无序访问视图组合类型描述符
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// 指定标记，着色器可见
	HeapDesc.NodeMask = 0;
	// 创建描述符堆
	GetD3dDevice()->CreateDescriptorHeap(
		&HeapDesc,
		IID_PPV_ARGS(&CBVHeap)
	);

	ObjectConstants = std::make_shared<FRenderingResourcesUpdate>();
	ObjectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
	D3D12_GPU_VIRTUAL_ADDRESS ObAddr = ObjectConstants.get()->GetBuffer()->GetGPUVirtualAddress();

	// 常量缓冲区描述
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDes;
	cbvDes.BufferLocation = ObAddr;		// 常量缓冲区地址
	cbvDes.SizeInBytes = ObjectConstants->GetConstantsBufferByteSize();		// 获取常量缓冲区字节大小

	GetD3dDevice()->CreateConstantBufferView(&cbvDes, CBVHeap->GetCPUDescriptorHandleForHeapStart());

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///构建根签名

	// CBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCBV;	// 常量缓冲区区描述符范围 描述符范围（Descriptor Range）的创建
	DescriptorRangeCBV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV,	// 指定视图（这里指向常量缓冲区视图 （描述符类型））
		1,									// 描述数量 1
		0);						// 基于那个着色器的寄存器（绑定寄存器（shaderRegister 和 registerSpace））

	// 创建根参数，使用上面的描述符范围
	CD3DX12_ROOT_PARAMETER RootParam[1];
	RootParam[0].InitAsDescriptorTable(
		1,							// 描述符数量
		&DescriptorRangeCBV,		// 指向描述符范围数组的指针
		D3D12_SHADER_VISIBILITY_ALL	// 着色器可见性(该值默认为shader可见，一般不用设置）
	);

	// 序列化根签名，将我们当前的描述二进制连续的一个内存(将根签名（Root Signature）序列化为字节流数据)

	// 根签名（Root Signature）描述结构体的创建
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		1,			// 参数数量
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

	GPUVertexBufferPtr = ConstructDefaultBuffer(TempVertexBufferPtr, InRenderingData->VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer(TempIndexBufferPtr, InRenderingData->IndexData.data(), IndexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));	// 创建一个二进制的缓冲区

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///PSO渲染流水线绑定
	
	// 描述当前渲染管线状态
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc{};

	// 绑定输入布局
	GPSDesc.InputLayout.NumElements = static_cast<UINT>(InputElementDesc.size());
	GPSDesc.InputLayout.pInputElementDescs = InputElementDesc.data();

	// 绑定输入布局
	GPSDesc.pRootSignature = RootSignature.Get();        // 根签名

	// 绑定着色器
	GPSDesc.VS.pShaderBytecode = static_cast<BYTE*>(VertexShader.GetBufferPointer());                   // 顶点着色器
	GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
	GPSDesc.PS.pShaderBytecode = static_cast<BYTE*>(PixelShader.GetBufferPointer());                   // 像素着色器
	GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();

	// 配置光栅化状态
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);		// 光栅化状态

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
	GPSDesc.SampleDesc.Count = GetEngine()->GetDXGISampleCount();		// 采样数量
	GPSDesc.SampleDesc.Quality = GetEngine()->GetDXGISampleQuality();	// 采样质量
	GPSDesc.RTVFormats[0] = GetEngine()->GetBackBufferFormat();			// 渲染目标视图格式（后台缓冲区格式）
	GPSDesc.DSVFormat = GetEngine()->GetDepthStencilFormat();			// 深度模板缓冲区格式

	// 通过D3D创建渲染管线状态对象
	ANALYSIS_RESULT(GetD3dDevice()->CreateGraphicsPipelineState(
		&GPSDesc,
		IID_PPV_ARGS(&PSO)
	));

}

FMesh* FMesh::CreateMesh(const FMeshRendingData* InRenderingData)
{
	FMesh* InMesh = new FMesh();

	InMesh->BuildMesh(InRenderingData);

	return InMesh;
}

D3D12_VERTEX_BUFFER_VIEW FMesh::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW vbv;

	vbv.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	vbv.SizeInBytes = VertexSizeInBytes;
	vbv.StrideInBytes = VertexStrideInBytes;

	return vbv;

}

D3D12_INDEX_BUFFER_VIEW FMesh::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW ibv;

	ibv.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
	ibv.SizeInBytes = IndexSizeInBytes;
	ibv.Format = IndexFormat;

	return ibv;
}
