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

// 2D向量
struct FFBXVector2
{
	float x = 0;
	float y = 0;
};

// 3D向量
struct FFBXVector3 : public  FFBXVector2
{
	float z = 0;
};

// 顶点数据
struct FFBXVertex
{
	FFBXVector3 Position{};
	FFBXVector3 Color{};
	FFBXVector3 Normal{};
	FFBXVector3 Tangent{};
	FFBXVector3 UV{};
	FFBXVector3 Binormal{};
};

// 图元信息（三角形图元信息）
struct FFBXTrianglePolygon
{
	FFBXVertex Vertexs[3];
	// 在FBX中，每一个图元都可以有自己的材质信息
	int MaterialID = 0;
};

// mesh
struct FFBXMesh
{
	std::vector<FFBXTrianglePolygon> VertexData{};
	std::vector<uint16_t> IndexData{};
	int MaterialID = 0;	// 主材质ID
};

// 材质
struct FFBXMaterial
{
	std::string DiffuseMapFileName;	// 漫反射材质
	std::string SpecularMapFileName;	// 高光贴图材质
	std::string AlphaMapFileName;	// alpha材质
	std::string BumpMapFileName;	// bump材质
};

// model
struct FFBXModel
{
	std::vector<FFBXMesh> MeshData;
	// 模型的映射关系（比如我们在Maya中将多个模型合并为一个模型，多个模型他们有自己的材质，那么这里就需要记录或者读取识别这种映射关系，读取子模型的材质）
	std::map<int, FFBXMaterial> MaterialMap;
};


// 渲染数据
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

	// 使用递归的方式读取fbx模型节点数据
	void RecursiveLoadMesh(FbxNode* node, FFBXRenderData& MeshData);

	// 获取mesh数据
	void GetMesh(FbxNode* node, FFBXModel& MeshData);

	// 获取多边形数据
	void GetPolygons(FbxMesh* mesh, FFBXMesh& MeshData);

	// 获取材质
	void GetMaterial();

	// 获取顶点索引
	void GetIndex(FFBXMesh& mesh);

private:
	FbxManager* fbxManager = nullptr;
	FbxScene* fbxScene = nullptr;
};

