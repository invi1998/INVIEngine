#pragma once

struct FFBXRenderData
{
	// ��
	

	// ����

	// ����

	// UV
};

class CFBXAssetImport
{
public:
	CFBXAssetImport() = default;

	void LoadMeshData(const std::string& path, FFBXRenderData& outData);

private:
	void InitializeSDKObjects(FbxManager*& manager, FbxScene*& scene);
};

