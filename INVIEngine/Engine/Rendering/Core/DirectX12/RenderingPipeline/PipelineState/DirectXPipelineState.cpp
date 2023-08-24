#include "DirectXPipelineState.h"

#include "Shader/Core/Shader.h"
#include "Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"

FDirectXPipelineState::FDirectXPipelineState()
{

}

void FDirectXPipelineState::BuildPipeline()
{
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

void FDirectXPipelineState::ResetGPSDesc()
{
	memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
}

void FDirectXPipelineState::BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InputElementDesc, UINT Size)
{
	// �����벼��
	GPSDesc.InputLayout.pInputElementDescs = InputElementDesc;
	GPSDesc.InputLayout.NumElements = Size;
}

void FDirectXPipelineState::BindRootSignature(ID3D12RootSignature* RootSignature)
{
	// �󶨸�ǩ��
	GPSDesc.pRootSignature = RootSignature;
}

void FDirectXPipelineState::BindShader(const FShader& InVertexShader, const FShader& InPixelShader)
{
	// ��shader
	GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(InVertexShader.GetBufferPointer());
	GPSDesc.VS.BytecodeLength = InVertexShader.GetBufferSize();

	GPSDesc.PS.pShaderBytecode = InPixelShader.GetBufferPointer();
	GPSDesc.PS.BytecodeLength = InPixelShader.GetBufferSize();
}

void FDirectXPipelineState::PreDraw(float DeltaTime)
{
	// ���������б�����Ϊ����ÿһ֡�������µ��ύ�б�
	GetD3dGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());
}

void FDirectXPipelineState::Draw(float DeltaTime)
{
}

void FDirectXPipelineState::PostDraw(float DeltaTime)
{
}