#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{
}

void FDirectXRootSignature::BuildRootSignature()
{
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
