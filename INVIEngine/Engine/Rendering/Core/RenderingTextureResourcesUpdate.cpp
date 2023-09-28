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

	// ��ȡDDS��ͼ����
	CreateDDSTextureFromFile12(
		GetD3dDevice().Get(),		// �豸����
		GetD3dGraphicsCommandList().Get(),	// �����б�
		Texture->Filename.c_str(),	// ��Դ·��
		Texture->Data,	// ��Դ��ַ
		Texture->UploadBuffer		// �ϴ�������
	);

	Texture->RenderingTextureID = TextureUnorderedMap.size();
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
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc = {};

	ShaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// �����������ݸ�ʽ
	ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// ����SRV��ά�ȣ�2D����
	ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;		// ��ɫ�����ӳ��Ľṹ�塣����������ָ����ͬ��ɫͨ��֮���ӳ���ϵ��
	// D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING��DXGI_ENUM�е�һ����������������Ĭ�ϵ���ɫ�����ӳ�䷽ʽ������أ�����ʾ��RGBA�ĸ���ɫͨ���ֱ�ӳ�䵽�������ݵ�RGBA�ĸ���ɫͨ�� (0, 1, 2, 3)
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0; // ָ��Ҫ����ɫ����ʹ�õ�����ϸ��mip�����������ͨ����0��ʼ����ʾ�������߷ֱ���mip����
	ShaderResourceViewDesc.Texture2D.MipLevels = 1; // ָ��ʹ�õ�mip��������������ڿ��������ϸ�ڼ���ͨ������Ϊmip������������������Ϊ-1�����ʾʹ�����п��õ�mip����
	ShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.f;	// �������ö�������Դ��LOD��Level of Detail�����ƣ�ָ��LOD����С����ֵ������������ɫ����������Դ�ķ��ʷ�Χ����������ڿ��������ϸ�ڼ���

	for (auto& val : TextureUnorderedMap | views::values)
	{
		ShaderResourceViewDesc.Format = val->Data->GetDesc().Format;
		ShaderResourceViewDesc.Texture2D.MipLevels = val->Data->GetDesc().MipLevels;

		GetD3dDevice()->CreateShaderResourceView(
			val->Data.Get(),		// ָ����Դ��ͼ
			&ShaderResourceViewDesc,			// shader��Դ����
			Handle
		);

		// ����һ����ͼ�󣬶Ծ������ һ��DescriptorOffset ��С���ڴ�ƫ��
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
		return &TextureUnorderedMap[texturePath];			// key����
	}
	else
	{
		for(auto &temp : TextureUnorderedMap)
		{
			if (temp.second->Filename == texturePath)	// ·��
			{
				return &temp.second;
			}
			if (temp.second->AssertFilename == texturePath)	// ��Դ·��
			{
				return &temp.second;
			}
			if (temp.second->SimpleAssertFilename == texturePath)	// ����Դ·��
			{
				return &temp.second;
			}
		}
	}

	return nullptr;
}
