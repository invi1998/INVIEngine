#include "RenderingTextureResourcesUpdate.h"

void FRenderingTextureResourcesUpdate::LoadTextureResource(const std::wstring& path)
{
	std::unique_ptr<FRenderingTexture> Texture = std::make_unique<FRenderingTexture>();
	Texture->Filename = path;
	wchar_t name[1024] = { 0 };
	get_path_clean_filename_w(name, Texture->Filename.c_str());
	Texture->Name = name;

	// 读取DDS贴图数据
	CreateDDSTextureFromFile12(
		GetD3dDevice().Get(),		// 设备驱动
		GetD3dGraphicsCommandList().Get(),	// 命令列表
		Texture->Filename.c_str(),	// 资源路径
		Texture->Data,	// 资源地址
		Texture->UploadBuffer		// 上传缓冲区
	);

	TextureUnorderedMap[Texture->Name] = std::move(Texture);

}

void FRenderingTextureResourcesUpdate::BuildTextureConstantBuffer(ID3D12DescriptorHeap* heap, int offset)
{
}
