#pragma once

struct FFBXRenderData
{
	// 点
	

	// 法线

	// 切线

	// UV
};

// FBX版本信息
struct FFBXVersion
{
	int Major;
	int Minor;
	int Revision;
	auto operator<=>(const FFBXVersion& fbxVersion) const;
};

class CFBXAssetImport
{
public:
	CFBXAssetImport();
	~CFBXAssetImport();

	void LoadMeshData(const std::string& path, FFBXRenderData& outData);

protected:
	void InitializeSDKObjects();

	void LoadScene(FbxDocument* scene, const char* fileName);

private:
	FbxManager* fbxManager = nullptr;
	FbxScene* fbxScene = nullptr;
};

