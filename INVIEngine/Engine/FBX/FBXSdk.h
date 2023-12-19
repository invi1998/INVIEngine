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

	auto operator<=>(const FFBXVersion& fbxVersion) const = default;
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

	// 使用递归的方式读取fbx模型节点数据
	void RecursiveLoadMesh(FbxNode* node, FFBXRenderData& outData);

	// 获取mesh数据
	void GetMesh(FbxNode* node, FFBXRenderData& outData);

	void GetPolygons(FbxMesh* mesh, FFBXRenderData& outData);

private:
	FbxManager* fbxManager = nullptr;
	FbxScene* fbxScene = nullptr;
};

