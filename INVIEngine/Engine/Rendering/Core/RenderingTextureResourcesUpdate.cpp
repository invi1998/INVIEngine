#include "RenderingTextureResourcesUpdate.h"

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
}
