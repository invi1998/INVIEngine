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

	// ���������б���Ϊ����ÿһ֡�������µ��ύ�б�
	GetD3dGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());
}

void CMeshManager::Draw(float DeltaTime)
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

	// ����ڶ���������������������Ӧ�ô��λ�õ�ƫ��
	int HeapOffset = 1;
	// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CBVHeap->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(0, DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesHandle);		// ����b0�Ĵ���

	DesHandle.Offset(1, DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);		// ����b1�Ĵ���

	// ����ͼԪ����ʵ��Ⱦģ�ͣ�
	GetD3dGraphicsCommandList()->DrawIndexedInstanced(
		IndexSize,		// ��������
		1,				// ��ʾҪ���Ƶ�ʵ���������ֻ��Ҫ���Ƶ���ʵ��������Խ��˲�������Ϊ 1��
		0,				// ��ʾ���������������ĸ�λ�ÿ�ʼ��ȡ�������ݡ�ͨ������£����ֵΪ 0��
		0,				// ��ʾÿ�������ڶ��㻺�������������ʼλ�õ�ƫ��������������������������еģ�����Խ��˲�������Ϊ 0��
		0				// ��ʾ�ӵڼ���ʵ����ʼ���ơ����ֻ��Ҫ���Ƶ���ʵ��������Խ��˲�������Ϊ 0��
	);
}

void CMeshManager::PostDraw(float DeltaTime)
{
	IRenderingInterface::PostDraw(DeltaTime);
}

void CMeshManager::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��������������

	// ������
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
	HeapDesc.NumDescriptors = 2;		// �������� 2
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;	// ָ�������ͣ�������������ͼ����ɫ����Դ��ͼ�����������ͼ�������������
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// ָ����ǣ���ɫ���ɼ�
	HeapDesc.NodeMask = 0;
	// ������������
	GetD3dDevice()->CreateDescriptorHeap(
		&HeapDesc,
		IID_PPV_ARGS(&CBVHeap)
	);

	// ����������1 object
	{
		// ���������������������� CBVHeap

		ObjectConstants = std::make_shared<FRenderingResourcesUpdate>();
		ObjectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
		D3D12_GPU_VIRTUAL_ADDRESS ObAddr = ObjectConstants.get()->GetBuffer()->GetGPUVirtualAddress();

		// ��������������
		D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDes;
		CBVDes.BufferLocation = ObAddr;		// ������������ַ
		CBVDes.SizeInBytes = ObjectConstants->GetConstantsBufferByteSize();		// ��ȡ�����������ֽڴ�С

		GetD3dDevice()->CreateConstantBufferView(&CBVDes, CBVHeap->GetCPUDescriptorHandleForHeapStart());
	}

	// ����������2 viewport
	{
		// �ӿڳ���������
		ViewportConstants = std::make_shared<FRenderingResourcesUpdate>();
		ViewportConstants->Init(GetD3dDevice().Get(), sizeof(FViewportTransformation), 1);
		D3D12_GPU_VIRTUAL_ADDRESS ViewportAddr = ViewportConstants.get()->GetBuffer()->GetGPUVirtualAddress();

		// ����ڶ���������������������Ӧ�ô��λ�õ�ƫ��
		int HeapOffset = 1;
		// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
		UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CBVHeap->GetCPUDescriptorHandleForHeapStart());
		DesHandle.Offset(HeapOffset, DescriptorOffset);

		// ��������������
		D3D12_CONSTANT_BUFFER_VIEW_DESC viewportCBVDes;
		viewportCBVDes.BufferLocation = ViewportAddr;		// ������������ַ
		viewportCBVDes.SizeInBytes = ViewportConstants->GetConstantsBufferByteSize();		// ��ȡ�����������ֽڴ�С
		// �������Ǽ����ƫ��ֵ��handle
		GetD3dDevice()->CreateConstantBufferView(&viewportCBVDes, DesHandle);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///������ǩ��

	// ��Ⱦ�������壩 CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjectCBV;	// ��������������������Χ ��������Χ��Descriptor Range���Ĵ���
	DescriptorRangeObjectCBV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV,	// ָ����ͼ������ָ������������ͼ �����������ͣ���
		1,									// �������� 1
		0);						// �����Ǹ���ɫ���ļĴ������󶨼Ĵ�����shaderRegister �� registerSpace����

	// �ӿ� CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;	// ��������������������Χ ��������Χ��Descriptor Range���Ĵ���
	DescriptorRangeViewportCBV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV,	// ָ����ͼ������ָ������������ͼ �����������ͣ���
		1,									// �������� 1
		1);						// �����Ǹ���ɫ���ļĴ������󶨼Ĵ�����shaderRegister �� registerSpace����

	// ������������ʹ���������������Χ
	CD3DX12_ROOT_PARAMETER RootParam[2];
	RootParam[0].InitAsDescriptorTable(
		1,							// ����������
		&DescriptorRangeObjectCBV		// ָ����������Χ�����ָ��
		// D3D12_SHADER_VISIBILITY_ALL	// ��ɫ���ɼ���(��ֵĬ��Ϊshader�ɼ���һ�㲻�����ã�
	);

	RootParam[1].InitAsDescriptorTable(
		1,							// ����������
		&DescriptorRangeViewportCBV		// ָ����������Χ�����ָ��
		// D3D12_SHADER_VISIBILITY_ALL	// ��ɫ���ɼ���(��ֵĬ��Ϊshader�ɼ���һ�㲻�����ã�
	);

	// ���л���ǩ���������ǵ�ǰ������������������һ���ڴ�(����ǩ����Root Signature�����л�Ϊ�ֽ�������)

	// ��ǩ����Root Signature�������ṹ��Ĵ���
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		2,			// ��������
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

	ConstructBuffer::FConstructDefaultBuffer ConstructDefaultBuffer;

	GPUVertexBufferPtr = ConstructDefaultBuffer.ConstructDefaultBuffer(TempVertexBufferPtr, InRenderingData->VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer.ConstructDefaultBuffer(TempIndexBufferPtr, InRenderingData->IndexData.data(), IndexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));	// ����һ�������ƵĻ�����

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///PSO��Ⱦ��ˮ�߰�

	// ������ǰ��Ⱦ����״̬
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc{};

	// �����벼��
	GPSDesc.InputLayout.NumElements = static_cast<UINT>(InputElementDesc.size());
	GPSDesc.InputLayout.pInputElementDescs = InputElementDesc.data();

	// �󶨸�ǩ��
	GPSDesc.pRootSignature = RootSignature.Get();        // ��ǩ��

	// ����ɫ��
	GPSDesc.VS.pShaderBytecode = static_cast<BYTE*>(VertexShader.GetBufferPointer());                   // ������ɫ��
	GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
	GPSDesc.PS.pShaderBytecode = PixelShader.GetBufferPointer();                   // ������ɫ��
	GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();

	// ���ù�դ��״̬
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);		// ��դ��״̬
	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;			// ���߿�ʽ��ʾ

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
	GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetDXGISampleCount();		// ��������
	GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetDXGISampleQuality();	// ��������
	GPSDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat();			// ��ȾĿ����ͼ��ʽ����̨��������ʽ��
	GPSDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilFormat();			// ���ģ�建������ʽ

	// ͨ��D3D������Ⱦ����״̬����
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

	// �����ӿ�
	XMMATRIX ProjectionMatrix = XMLoadFloat4x4(&viewport_info.ProjectionMatrix);	// ͶӰ����
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&viewport_info.ViewMatrix);				// �ӿھ���

	XMMATRIX MatrixWorld = XMLoadFloat4x4(&WorldMatrix);

	FObjectTransformation OBJTransformation;
	XMStoreFloat4x4(&OBJTransformation.World, XMMatrixTranspose(MatrixWorld));

	ObjectConstants->Update(0, &OBJTransformation);


	XMMATRIX ViewProjection= XMMatrixMultiply(ViewMatrix, ProjectionMatrix);

	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProjection));	// �洢֮ǰ�ǵöԾ������ת��

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

	// ��ȡģ����Դ
	FMeshRenderingData MeshData;
	MyMesh->CreateMesh(MeshData, std::forward<ParamTypes>(Params)...);

	MyMesh->BeginInit();

	// ����mesh
	// BuildMesh(&MeshData);
	RenderingPipeline.BuildMesh(MyMesh, &MeshData);

	MyMesh->Init();

	return MyMesh;
}
