#pragma once

struct FFBXRenderData
{
	// ��
	

	// ����

	// ����

	// UV
};

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

	void LoadMeshData(const std::string& path, FFBXRenderData& outData);

protected:
	void InitializeSDKObjects();

	void LoadScene(FbxDocument* scene, const char* fileName);

	// ʹ�õݹ�ķ�ʽ��ȡfbxģ�ͽڵ�����
	void RecursiveLoadMesh(FbxNode* node, FFBXRenderData& outData);

	// ��ȡmesh����
	void GetMesh(FbxNode* node, FFBXRenderData& outData);

	void GetPolygons(FbxMesh* mesh, FFBXRenderData& outData);

private:
	FbxManager* fbxManager = nullptr;
	FbxScene* fbxScene = nullptr;
};

