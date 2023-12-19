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
	// ��������
	fbxManager = FbxManager::Create();
	// ���ڸ�io����io
	FbxIOSettings* fbxIoSetting = FbxIOSettings::Create(fbxManager, IOSROOT);
	// ������io���ٷ���ע���manager��
	fbxManager->SetIOSettings(fbxIoSetting);
	// ��ȡӦ�ó���Ĺ���·��
	FbxString fbxPath = FbxGetApplicationDirectory();
	// ������·��ע���manager��
	fbxManager->LoadPluginsDirectory(fbxPath);

	// ��������
	fbxScene = FbxScene::Create(fbxManager, "INVI_FBX_SCENE");
}
