#pragma once

struct FMeshRenderingData;

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

	void LoadMeshData(const std::string& path, FMeshRenderingData& MeshData);

protected:
	void InitializeSDKObjects();

	void LoadScene(FbxDocument* scene, const char* fileName);

	// 使用递归的方式读取fbx模型节点数据
	void RecursiveLoadMesh(FbxNode* node, FMeshRenderingData& MeshData);

	// 获取mesh数据
	void GetMesh(FbxNode* node, FMeshRenderingData& MeshData);

	void GetPolygons(FbxMesh* mesh, FMeshRenderingData& MeshData);

private:
	FbxManager* fbxManager = nullptr;
	FbxScene* fbxScene = nullptr;
};

