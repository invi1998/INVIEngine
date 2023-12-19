#pragma once

struct FFBXRenderData
{
	// 点
	

	// 法线

	// 切线

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

