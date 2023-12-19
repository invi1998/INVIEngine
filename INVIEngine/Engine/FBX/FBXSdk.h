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

// 2D����
struct FFBXVector2
{
	float x = 0;
	float y = 0;
};

// 3D����
struct FFBXVector3 : public  FFBXVector2
{
	float z = 0;
};

// ��������
struct FFBXVertex
{
	FFBXVector3 Position{};
	FFBXVector3 Color{};
	FFBXVector3 Normal{};
	FFBXVector3 Tangent{};
	FFBXVector3 UV{};
	FFBXVector3 Binormal{};
};

// ͼԪ��Ϣ��������ͼԪ��Ϣ��
struct FFBXTrianglePolygon
{
	FFBXVertex Vertexs[3];
	// ��FBX�У�ÿһ��ͼԪ���������Լ��Ĳ�����Ϣ
	int MaterialID = 0;
};

// mesh
struct FFBXMesh
{
	std::vector<FFBXTrianglePolygon> VertexData{};
	std::vector<uint16_t> IndexData{};
	int MaterialID = 0;	// ������ID
};

// ����
struct FFBXMaterial
{
	std::string DiffuseMapFileName;	// ���������
	std::string SpecularMapFileName;	// �߹���ͼ����
	std::string AlphaMapFileName;	// alpha����
	std::string BumpMapFileName;	// bump����
};

// model
struct FFBXModel
{
	std::vector<FFBXMesh> MeshData;
	// ģ�͵�ӳ���ϵ������������Maya�н����ģ�ͺϲ�Ϊһ��ģ�ͣ����ģ���������Լ��Ĳ��ʣ���ô�������Ҫ��¼���߶�ȡʶ������ӳ���ϵ����ȡ��ģ�͵Ĳ��ʣ�
	std::map<int, FFBXMaterial> MaterialMap;
};


// ��Ⱦ����
struct FFBXRenderData
{
	std::vector<FFBXModel> ModelData;
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
	void RecursiveLoadMesh(FbxNode* node, FFBXRenderData& MeshData);

	// ��ȡmesh����
	void GetMesh(FbxNode* node, FFBXModel& MeshData);

	// ��ȡ���������
	void GetPolygons(FbxMesh* mesh, FFBXMesh& MeshData);

	// ��ȡ����
	void GetMaterial();

	// ��ȡ��������
	void GetIndex(FFBXMesh& mesh);

private:
	FbxManager* fbxManager = nullptr;
	FbxScene* fbxScene = nullptr;
};

