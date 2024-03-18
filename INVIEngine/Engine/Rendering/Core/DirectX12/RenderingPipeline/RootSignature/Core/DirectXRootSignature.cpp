#include "EngineMinimal.h"

#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{
}

void FDirectXRootSignature::BuildRootSignature(UINT textureNum)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///������ǩ��

	// ������������ʹ���������������Χ
	CD3DX12_ROOT_PARAMETER RootParam[10];

	// ���� CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;	// ��������������������Χ ��������Χ��Descriptor Range���Ĵ���
	DescriptorRangeTextureSRV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,	// ָ����ͼ������ָ������������ͼ �����������ͣ���������������������ѡ��shaderRenderingView
		textureNum,							// ��ͼ����
		4);						// �����Ǹ���ɫ���ļĴ������󶨼Ĵ�����shaderRegister �� registerSpace����

	// ��̬CubeMap CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCubeMapSRV;
	DescriptorRangeCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ������Ӱ��ͼ CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorCubeMapShadowSRV;
	DescriptorCubeMapShadowSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

	// ��Ӱ��ͼ CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeShadowMapSRV;
	DescriptorRangeShadowMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

	// SSAO CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeSSAOSRV;
	DescriptorRangeSSAOSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);

	// register(t0, space1)
	RootParam[0].InitAsConstantBufferView(0);		// Mesh����
	RootParam[1].InitAsConstantBufferView(1);		// �ӿ�
	RootParam[2].InitAsConstantBufferView(2);		// �ƹ�
	RootParam[3].InitAsConstantBufferView(3);		// ��
	// register(t0, space1)
	RootParam[4].InitAsShaderResourceView(0, 1);		// ����


	// 2D��ͼ
	RootParam[5].InitAsDescriptorTable(
		1,								// ����������
		&DescriptorRangeTextureSRV,			// ָ����������Χ�����ָ��
		D3D12_SHADER_VISIBILITY_PIXEL	// ��ɫ���ɼ���(��ֵĬ��Ϊshader�ɼ���һ�㲻�����ã�
	);

	// cube map
	RootParam[6].InitAsDescriptorTable(
		1,								// ����������
		&DescriptorRangeCubeMapSRV,			// ָ����������Χ�����ָ��
		D3D12_SHADER_VISIBILITY_PIXEL	// ��ɫ���ɼ���(��ֵĬ��Ϊshader�ɼ���һ�㲻�����ã�
	);

	// Shadow Map
	RootParam[7].InitAsDescriptorTable(
		1,								// ����������
		&DescriptorRangeShadowMapSRV,			// ָ����������Χ�����ָ��
		D3D12_SHADER_VISIBILITY_PIXEL	// ��ɫ���ɼ���(��ֵĬ��Ϊshader�ɼ���һ�㲻�����ã�
	);

	// CubeMap Shadow
	RootParam[8].InitAsDescriptorTable(
		1,								// ����������
		&DescriptorCubeMapShadowSRV,			// ָ����������Χ�����ָ��
		D3D12_SHADER_VISIBILITY_PIXEL	// ��ɫ���ɼ���(��ֵĬ��Ϊshader�ɼ���һ�㲻�����ã�
	);

	// SSAO
	RootParam[9].InitAsDescriptorTable(
		1,								// ����������
		&DescriptorRangeSSAOSRV,			// ָ����������Χ�����ָ��
		D3D12_SHADER_VISIBILITY_PIXEL	// ��ɫ���ɼ���(��ֵĬ��Ϊshader�ɼ���һ�㲻�����ã�
	);

	// ���л���ǩ���������ǵ�ǰ������������������һ���ڴ�(����ǩ����Root Signature�����л�Ϊ�ֽ�������)

	// ���ò�����ʽ-��̬������ʽ
	StaticSamplerObject.BuildStaticSampler();

	// ��ǩ����Root Signature�������ṹ��Ĵ���
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		10,			// ��������
		RootParam,	// ��ǩ������
		StaticSamplerObject.GetSize(),			// ��̬��������
		StaticSamplerObject.GetData(),			// ��̬�������ݣ������������ָ�룩
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
}

void FDirectXRootSignature::PreDraw(float DeltaTime)
{
	// ���ø�ǩ��
	GetD3dGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());
}

void FDirectXRootSignature::Draw(float DeltaTime)
{
}

void FDirectXRootSignature::PostDraw(float DeltaTime)
{
}
