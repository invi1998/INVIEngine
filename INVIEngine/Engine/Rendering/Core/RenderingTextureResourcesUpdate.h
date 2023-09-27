#pragma once

#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"

struct FRenderingTexture
{
	FRenderingTexture()
		: RenderingTextureID(0)
	{}

	ComPtr<ID3D12Resource> UploadBuffer;		// �ϴ���
	ComPtr<ID3D12Resource> Data;				// ������Դ
	std::wstring Name;							// ��������
	std::wstring Filename;						// ��Դ·��(ʹ�ÿ��ַ��������������·���ַ���ȡ���������⣩

	UINT RenderingTextureID;
};

class FRenderingTextureResourcesUpdate : public std::enable_shared_from_this<FRenderingTextureResourcesUpdate>, public IDirectXDeviceInterface
{
public:
	void LoadTextureResource(const std::wstring& path);		// ��������
	void BuildTextureConstantBuffer(ID3D12DescriptorHeap* heap, int offset = 0);	// ����������Դ����������

protected:
	// ��ͼ���֣���ͼ��Դָ��
	std::unordered_map<std::wstring, std::unique_ptr<FRenderingTexture>> TextureUnorderedMap;
};

