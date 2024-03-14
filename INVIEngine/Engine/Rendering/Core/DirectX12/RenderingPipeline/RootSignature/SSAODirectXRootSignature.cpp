#include "EngineMinimal.h"
#include "SSAODirectXRootSignature.h"

ID3D12RootSignature* FSSAODirectXRootSignature::GetRootSignature() const
{
	return FDirectXRootSignature::GetRootSignature();
}

void FSSAODirectXRootSignature::BuildRootSignature(UINT textureNum)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///������ǩ��
	///
	///// ������������ʹ���������������Χ
	CD3DX12_ROOT_PARAMETER RootParam[1];

	// ���� CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;	// ��������������������Χ ��������Χ��Descriptor Range���Ĵ���
	DescriptorRangeTextureSRV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,	// ָ����ͼ������ָ������������ͼ �����������ͣ���������������������ѡ��shaderRenderingView
		textureNum,							// ��ͼ����
		0);						// �����Ǹ���ɫ���ļĴ������󶨼Ĵ�����shaderRegister �� registerSpace����

	// ��̬CubeMap CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeNormalSRV;
	DescriptorRangeNormalSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// register(t0, space1)
	RootParam[0].InitAsConstantBufferView(0);		// Normal

	// ���л���ǩ���������ǵ�ǰ������������������һ���ڴ�(����ǩ����Root Signature�����л�Ϊ�ֽ�������)

	// ���ò�����ʽ-��̬������ʽ
	GetStaticSampler().BuildStaticSampler();



	// ��ǩ����Root Signature�������ṹ��Ĵ���
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		9,			// ��������
		RootParam,	// ��ǩ������
		GetStaticSampler().GetSize(),			// ��̬��������
		GetStaticSampler().GetData(),			// ��̬�������ݣ������������ָ�룩
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
		IID_PPV_ARGS(&GetRootSignatureComPtr())							// �����ǩ�����յ�ַ
	);
}

void FSSAODirectXRootSignature::PreDraw(float DeltaTime)
{
	FDirectXRootSignature::PreDraw(DeltaTime);
}

void FSSAODirectXRootSignature::Draw(float DeltaTime)
{
	FDirectXRootSignature::Draw(DeltaTime);
}

void FSSAODirectXRootSignature::PostDraw(float DeltaTime)
{
	FDirectXRootSignature::PostDraw(DeltaTime);
}
