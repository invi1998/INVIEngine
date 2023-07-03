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

	// �����ǵĶ��㻺��������Ⱦ��ˮ�߰�

	// ��ȡ���㻺������ͼ
	D3D12_VERTEX_BUFFER_VIEW vbv = GetVertexBufferView();
	// ����Ⱦ��ˮ���ϵ�����ۣ�����������װ�����׶δ��붥������
	GetD3dGraphicsCommandList()->IASetVertexBuffers(
		0,		// ָ����ʼ�Ķ��㻺�����ۺ� `StartSlot`�����ֵͨ��Ϊ 0 ֵ��Χ 0-15
		1,		// ָ��Ҫ�󶨵Ķ��㻺�������� NumViews�����ֵ����С�ڻ����Ӳ��֧�ֵ���󶥵㻺��������
		&vbv	// ʹ�ö��㻺������ͼ���� pViews ������ÿ�����㻺������λ�á���С�Ͳ��ֵ���Ϣ��
	);

	D3D12_INDEX_BUFFER_VIEW ibv = GetIndexBufferView();
	GetD3dGraphicsCommandList()->IASetIndexBuffer(
		&ibv	// ������������ͼ
	);

	// ��������ֻ�����ύ���ݵ�GPU������������Ⱦģ��

	// ָ��ͼԪ���ͣ��㣬�ߣ��棩 ��������Ϊ �����������ι��ɵ��б�
	GetD3dGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

	// ����ͼԪ����ʵ��Ⱦģ�ͣ�
	GetD3dGraphicsCommandList()->DrawIndexedInstanced(
		IndexSize,		// ��������
		1,				// ��ʾҪ���Ƶ�ʵ���������ֻ��Ҫ���Ƶ���ʵ��������Խ��˲�������Ϊ 1��
		0,				// ��ʾ���������������ĸ�λ�ÿ�ʼ��ȡ�������ݡ�ͨ������£����ֵΪ 0��
		0,				// ��ʾÿ�������ڶ��㻺�������������ʼλ�õ�ƫ��������������������������еģ�����Խ��˲�������Ϊ 0��
		0				// ��ʾ�ӵڼ���ʵ����ʼ���ơ����ֻ��Ҫ���Ƶ���ʵ��������Խ��˲�������Ϊ 0��
	);
}

void FMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��������������

	// ���������������������� CBVHeap

	// ������
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
	HeapDesc.NumDescriptors = 1;		// �������� 1
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;	// ָ�������ͣ�������������ͼ����ɫ����Դ��ͼ�����������ͼ�������������
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// ָ����ǣ���ɫ���ɼ�
	HeapDesc.NodeMask = 0;
	// ������������
	GetD3dDevice()->CreateDescriptorHeap(
		&HeapDesc,
		IID_PPV_ARGS(&CBVHeap)
	);

	ObjectConstants = std::make_shared<FRenderingResourcesUpdate>();
	ObjectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
	D3D12_GPU_VIRTUAL_ADDRESS ObAddr = ObjectConstants.get()->GetBuffer()->GetGPUVirtualAddress();

	// ��������������
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDes;
	cbvDes.BufferLocation = ObAddr;		// ������������ַ
	cbvDes.SizeInBytes = ObjectConstants->GetConstantsBufferByteSize();		// ��ȡ�����������ֽڴ�С

	GetD3dDevice()->CreateConstantBufferView(&cbvDes, CBVHeap->GetCPUDescriptorHandleForHeapStart());

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///������ǩ��

	// CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCBV;	// ��������������������Χ ��������Χ��Descriptor Range���Ĵ���
	DescriptorRangeCBV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV,	// ָ����ͼ������ָ������������ͼ �����������ͣ���
		1,									// �������� 1
		0);						// �����Ǹ���ɫ���ļĴ������󶨼Ĵ�����shaderRegister �� registerSpace����

	// ������������ʹ���������������Χ
	CD3DX12_ROOT_PARAMETER RootParam[1];
	RootParam[0].InitAsDescriptorTable(
		1,							// ����������
		&DescriptorRangeCBV,		// ָ����������Χ�����ָ��
		D3D12_SHADER_VISIBILITY_ALL	// ��ɫ���ɼ���(��ֵĬ��Ϊshader�ɼ���һ�㲻�����ã�
	);

	// ���л���ǩ���������ǵ�ǰ������������������һ���ڴ�(����ǩ����Root Signature�����л�Ϊ�ֽ�������)

	// ��ǩ����Root Signature�������ṹ��Ĵ���
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		1,			// ��������
		RootParam,	// ��ǩ������
		0,			// ��̬��������
		nullptr,	// ��̬��������
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT // ָ����ǩ������ѡ�� ��ʾ��ǩ�����������������ʸ��������ݡ�
	);

	// ����������Խ���ǩ�����л�Ϊһ�� ID3DBlob ���󣬸ö��������һ���ֽ��������������ں����Ĳ����д��ݡ�����ͼ��ظ�ǩ����
	ComPtr<ID3DBlob> SerializeRootSignatureBlob;	// �洢���л��ɹ��Ķ�����������
	ComPtr<ID3DBlob> ErrorBlob;						// �洢���л�ʧ�ܵ���Ϣ
	D3D12SerializeRootSignature(
		&RootSignatureDesc,							// ����Ҫ���л��ĸ�ǩ�������ṹ��ָ��
		D3D_ROOT_SIGNATURE_VERSION_1,				// ��ǩ���İ汾��
		SerializeRootSignatureBlob.GetAddressOf(),	// ���л���ĸ�ǩ������
		ErrorBlob.GetAddressOf()					// �ö�������������л������г��ִ���ʱ�����Ĵ�����Ϣ
	);

	if (ErrorBlob)
	{
		ENGINE_LOG_ERROR("%s", static_cast<char*>(ErrorBlob->GetBufferPointer()));
	}

	// �ɹ���������ǩ��
	GetD3dDevice()->CreateRootSignature(
		0,			// ��ʾҪ�����ĸ�ǩ�����������Ľڵ����룬ͨ������Ϊ 0, ��ʾ����CPU��(�豸��������������
		SerializeRootSignatureBlob->GetBufferPointer(),			// ��ʾ�������л���ǩ�����ݵ��ڴ��ַ��
		SerializeRootSignatureBlob->GetBufferSize(),			// ��ʾ���л���ǩ�����ݵ��ֽڳ��ȡ�
		IID_PPV_ARGS(&RootSignature)							// �����ǩ�����յ�ַ
	);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///����shader HLSL
	VertexShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "VSMain", "vs_5_0");
	PixelShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "PSMain", "ps_5_0");

	InputElementDesc = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	// ��ɫ���������ƫ����12�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ�������Ե�����ɫ������� 3*4 = 12�ֽڵ�ƫ����
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����ģ��

	IndexSize = InRenderingData->IndexData.size();
	VertexStrideInBytes = sizeof(FVertex);

	// ���㶥�������ֽڴ�С
	VertexSizeInBytes = InRenderingData->VertexData.size() * sizeof(FVertex);
	IndexSizeInBytes = InRenderingData->IndexData.size() * sizeof(uint16_t);

	// ����������
	ANALYSIS_RESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));	// ����һ�������ƵĻ�����
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), VertexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData->IndexData.data(), IndexSizeInBytes);

	GPUVertexBufferPtr = ConstructDefaultBuffer(TempVertexBufferPtr, InRenderingData->VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer(TempIndexBufferPtr, InRenderingData->IndexData.data(), IndexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));	// ����һ�������ƵĻ�����

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///PSO��Ⱦ��ˮ�߰�
	
	// ������ǰ��Ⱦ����״̬
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc{};

	// �����벼��
	GPSDesc.InputLayout.NumElements = static_cast<UINT>(InputElementDesc.size());
	GPSDesc.InputLayout.pInputElementDescs = InputElementDesc.data();

	// �����벼��
	GPSDesc.pRootSignature = RootSignature.Get();        // ��ǩ��

	// ����ɫ��
	GPSDesc.VS.pShaderBytecode = static_cast<BYTE*>(VertexShader.GetBufferPointer());                   // ������ɫ��
	GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
	GPSDesc.PS.pShaderBytecode = static_cast<BYTE*>(PixelShader.GetBufferPointer());                   // ������ɫ��
	GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();

	// ���ù�դ��״̬
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);		// ��դ��״̬

	GPSDesc.SampleMask = UINT_MAX;		// ���ز������֣����״̬�µ�ʵ���ɰ棩���ز����£�����ǿ��Բ�32�������壨000...000),ÿ��0��ʾһ������λ����32����������ĸ���������λΪ1��UINT_MAX��ʾ0xffffffff����ȫ��������

	// ָ��ͼԪ��������
	GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;		// ������

	// ָ����ȾĿ��
	GPSDesc.NumRenderTargets = 1;

	// ָ�����״̬
	GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);		// Ĭ�ϻ��

	// �������ģ��
	GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	// Ĭ�����ģ��

	// ���ò���
	GPSDesc.SampleDesc.Count = GetEngine()->GetDXGISampleCount();		// ��������
	GPSDesc.SampleDesc.Quality = GetEngine()->GetDXGISampleQuality();	// ��������
	GPSDesc.RTVFormats[0] = GetEngine()->GetBackBufferFormat();			// ��ȾĿ����ͼ��ʽ����̨��������ʽ��
	GPSDesc.DSVFormat = GetEngine()->GetDepthStencilFormat();			// ���ģ�建������ʽ

	// ͨ��D3D������Ⱦ����״̬����
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
