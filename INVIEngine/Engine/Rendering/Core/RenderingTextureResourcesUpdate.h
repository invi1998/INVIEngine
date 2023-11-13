#pragma once

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
	std::wstring AssertFilename;				// ��Դ·�� Texture'Asserts/Texture/girls.dds'
	std::wstring SimpleAssertFilename;			// ����Դ·�� Asserts/Texture/girls.dds

	UINT RenderingTextureID;
};

class FRenderingTextureResourcesUpdate : public std::enable_shared_from_this<FRenderingTextureResourcesUpdate>, public IDirectXDeviceInterface
{
public:
	FRenderingTextureResourcesUpdate();

	void LoadTextureResource(const std::wstring& path);		// ��������
	void BuildTextureConstantBuffer(ID3D12DescriptorHeap* heap, int offset = 0);	// ����������Դ����������

	UINT Size() const { return TextureMap.size(); }

	std::unique_ptr<FRenderingTexture>* FindRenderingTexture(const std::string& key);

	void BuildParam();
	void ResetTextureByType(std::unique_ptr<FRenderingTexture>* texturePtr);

	void SetViewDimension(D3D12_SRV_DIMENSION dimension);

protected:
	// ��ͼ���֣���ͼ��Դָ��
	std::map<std::wstring, std::unique_ptr<FRenderingTexture>> TextureMap{};

	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc{};

};

