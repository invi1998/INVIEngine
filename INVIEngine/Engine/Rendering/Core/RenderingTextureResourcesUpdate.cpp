#include "EngineMinimal.h"

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

	Texture->RenderingTextureID = TextureUnorderedMap.size();
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
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc = {};

	ShaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// 设置纹理数据格式
	ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 设置SRV的维度，2D纹理
	ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;		// 着色器组件映射的结构体。它可以用于指定不同颜色通道之间的映射关系，
	// D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING是DXGI_ENUM中的一个常量，它代表了默认的着色器组件映射方式。具体地，它表示将RGBA四个颜色通道分别映射到输入数据的RGBA四个颜色通道 (0, 1, 2, 3)
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0; // 指定要在着色器中使用的最详细的mip级别的索引，通常从0开始，表示纹理的最高分辨率mip级别
	ShaderResourceViewDesc.Texture2D.MipLevels = 1; // 指定使用的mip级别的数量，用于控制纹理的细节级别，通常设置为mip级别的数量，如果设置为-1，则表示使用所有可用的mip级别。
	ShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.f;	// 用于设置对纹理资源的LOD（Level of Detail）限制，指定LOD的最小允许值，用于限制着色器对纹理资源的访问范围。这可以用于控制纹理的细节级别。

	for (auto& val : TextureUnorderedMap | views::values)
	{
		ShaderResourceViewDesc.Format = val->Data->GetDesc().Format;
		ShaderResourceViewDesc.Texture2D.MipLevels = val->Data->GetDesc().MipLevels;

		GetD3dDevice()->CreateShaderResourceView(
			val->Data.Get(),		// 指定资源视图
			&ShaderResourceViewDesc,			// shader资源描述
			Handle
		);

		// 存完一张贴图后，对句柄进行 一个DescriptorOffset 大小的内存偏移
		Handle.Offset(1, DescriptorOffset);
	}

}

std::unique_ptr<FRenderingTexture>* FRenderingTextureResourcesUpdate::FindRenderingTexture(const std::string& key)
{
	const char* instring = key.c_str();
	wchar_t texturePath[1024] = { 0 };
	char_to_wchar_t(texturePath, 1024, instring);

	if (TextureUnorderedMap.find(texturePath) != TextureUnorderedMap.end())
	{
		return &TextureUnorderedMap[texturePath];			// key查找
	}
	else
	{
		for(auto &temp : TextureUnorderedMap)
		{
			if (temp.second->Filename == texturePath)	// 路径
			{
				return &temp.second;
			}
			if (temp.second->AssertFilename == texturePath)	// 资源路径
			{
				return &temp.second;
			}
			if (temp.second->SimpleAssertFilename == texturePath)	// 简单资源路径
			{
				return &temp.second;
			}
		}
	}

	return nullptr;
}
