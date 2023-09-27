#pragma once

#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"

struct FRenderingTexture
{
	FRenderingTexture()
		: RenderingTextureID(0)
	{}

	ComPtr<ID3D12Resource> UploadBuffer;		// 上传堆
	ComPtr<ID3D12Resource> Data;				// 纹理资源
	std::wstring Name;							// 纹理名字
	std::wstring Filename;						// 资源路径(使用宽字符，避免出现中文路径字符读取不到的问题）

	UINT RenderingTextureID;
};

class FRenderingTextureResourcesUpdate : public std::enable_shared_from_this<FRenderingTextureResourcesUpdate>, public IDirectXDeviceInterface
{
public:
	void LoadTextureResource(const std::wstring& path);		// 加载纹理
	void BuildTextureConstantBuffer(ID3D12DescriptorHeap* heap, int offset = 0);	// 创建纹理资源常量缓冲区

protected:
	// 贴图名字：贴图资源指针
	std::unordered_map<std::wstring, std::unique_ptr<FRenderingTexture>> TextureUnorderedMap;
};

