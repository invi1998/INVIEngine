#include "RenderingTextureResourcesUpdate.h"

#include <ranges>

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
	// 获取描述符偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(heap->GetCPUDescriptorHandleForHeapStart());

	// 然后将句柄指针偏移到指定内存
	Handle.Offset(offset, DescriptorOffset);

	// Shader资源视图
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;

	ShaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;		// 设置纹理数据格式
	ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 设置SRV的维度，2D纹理

	for (auto& val : TextureUnorderedMap | views::values)
	{
		GetD3dDevice()->CreateShaderResourceView(
			val->UploadBuffer.Get(),		// 上传缓冲区地址
			&ShaderResourceViewDesc,			// shader资源描述
			Handle
		);

		// 存完一张贴图后，对句柄进行 一个DescriptorOffset 大小的内存偏移
		Handle.Offset(1, DescriptorOffset);
	}

}
