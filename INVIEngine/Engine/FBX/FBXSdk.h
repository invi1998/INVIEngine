#pragma once

struct FMeshRenderingData;

// FBX�汾��Ϣ
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

	// ʹ�õݹ�ķ�ʽ��ȡfbxģ�ͽڵ�����
	void RecursiveLoadMesh(FbxNode* node, FMeshRenderingData& MeshData);

	// ��ȡmesh����
	void GetMesh(FbxNode* node, FMeshRenderingData& MeshData);

	void GetPolygons(FbxMesh* mesh, FMeshRenderingData& MeshData);

private:
	FbxManager* fbxManager = nullptr;
	FbxScene* fbxScene = nullptr;
};

