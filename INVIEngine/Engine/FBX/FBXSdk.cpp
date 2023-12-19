#include "EngineMinimal.h"
#include "FBXSdk.h"


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
