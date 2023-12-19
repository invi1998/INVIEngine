#include "EngineMinimal.h"
#include "FBXSdk.h"


auto FFBXVersion::operator<=>(const FFBXVersion& fbxVersion) const
{
}

CFBXAssetImport::CFBXAssetImport()
{
	InitializeSDKObjects();
}

CFBXAssetImport::~CFBXAssetImport()
{
	delete fbxScene;
	delete fbxManager;
}

void CFBXAssetImport::LoadMeshData(const std::string& path, FFBXRenderData& outData)
{
	// FbxString fbxPath(path.c_str());
	// LoadScene(fbxScene, fbxPath.Buffer());

	LoadScene(fbxScene, path.c_str());
}

void CFBXAssetImport::InitializeSDKObjects()
{
	// 创建管理
	fbxManager = FbxManager::Create();
	// 基于根io创建io
	FbxIOSettings* fbxIoSetting = FbxIOSettings::Create(fbxManager, IOSROOT);
	// 创建好io后，再反向注册进manager里
	fbxManager->SetIOSettings(fbxIoSetting);
	// 获取应用程序的工作路径
	FbxString fbxPath = FbxGetApplicationDirectory();
	// 将工作路径注册进manager里
	fbxManager->LoadPluginsDirectory(fbxPath);

	// 创建场景
	fbxScene = FbxScene::Create(fbxManager, "INVI_FBX_SCENE");
}

void CFBXAssetImport::LoadScene(FbxDocument* scene, const char* fileName)
{
	// SDK版本信息
	FFBXVersion SDKVersion{};
	FbxManager::GetFileFormatVersion(SDKVersion.Major, SDKVersion.Minor, SDKVersion.Revision);

	// 创建导入器
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "INVI_FBX_IMPORTER");
	// 导入器初始化
	if (fbxImporter->Initialize(fileName, -1, fbxManager->GetIOSettings()))
	{
		FFBXVersion FbxAssetsVersion{};
		fbxImporter->GetFileVersion(FbxAssetsVersion.Major, FbxAssetsVersion.Minor, FbxAssetsVersion.Revision);

		// FBX资源和SDK版本进行比对
		if (SDKVersion < FbxAssetsVersion)
		{
			return;
		}
	}
}
