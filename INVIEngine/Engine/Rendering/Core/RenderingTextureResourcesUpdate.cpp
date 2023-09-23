#include "RenderingTextureResourcesUpdate.h"

#include <ranges>

void FRenderingTextureResourcesUpdate::LoadTextureResource(const std::wstring& path)
{
	std::unique_ptr<FRenderingTexture> Texture = std::make_unique<FRenderingTexture>();
	Texture->Filename = path;
	wchar_t name[1024] = { 0 };
	get_path_clean_filename_w(name, Texture->Filename.c_str());
	Texture->Name = name;

	// ��ȡDDS��ͼ����
	CreateDDSTextureFromFile12(
		GetD3dDevice().Get(),		// �豸����
		GetD3dGraphicsCommandList().Get(),	// �����б�
		Texture->Filename.c_str(),	// ��Դ·��
		Texture->Data,	// ��Դ��ַ
		Texture->UploadBuffer		// �ϴ�������
	);

	TextureUnorderedMap[Texture->Name] = std::move(Texture);

}

void FRenderingTextureResourcesUpdate::BuildTextureConstantBuffer(ID3D12DescriptorHeap* heap, int offset)
{
	// ��ȡ������ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(heap->GetCPUDescriptorHandleForHeapStart());

	// Ȼ�󽫾��ָ��ƫ�Ƶ�ָ���ڴ�
	Handle.Offset(offset, DescriptorOffset);

	// Shader��Դ��ͼ
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;

	ShaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;		// �����������ݸ�ʽ
	ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// ����SRV��ά�ȣ�2D����

	for (auto& val : TextureUnorderedMap | views::values)
	{
		GetD3dDevice()->CreateShaderResourceView(
			val->UploadBuffer.Get(),		// �ϴ���������ַ
			&ShaderResourceViewDesc,			// shader��Դ����
			Handle
		);

		// ����һ����ͼ�󣬶Ծ������ һ��DescriptorOffset ��С���ڴ�ƫ��
		Handle.Offset(1, DescriptorOffset);
	}

}
