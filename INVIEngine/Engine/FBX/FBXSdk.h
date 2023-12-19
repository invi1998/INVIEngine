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
	CFBXAssetImport();
	~CFBXAssetImport();

	void LoadMeshData(const std::string& path, FFBXRenderData& outData);

private:
	void InitializeSDKObjects();

	FbxManager* fbxManager = nullptr;
	FbxScene* fbxScene = nullptr;
};

