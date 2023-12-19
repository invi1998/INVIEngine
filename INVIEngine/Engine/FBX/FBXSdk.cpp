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

		// 判断是不是fbx资源
		if (!fbxImporter->IsFBX())
		{
			ENGINE_LOG_ERROR("当前资源不是fbx资源。%s", fbxImporter->GetFileName().Buffer());
			return;
		}

		// FBX资源和SDK版本进行比对
		if (SDKVersion < FbxAssetsVersion)
		{
			ENGINE_LOG_ERROR("SDK版本低于FBX资源版本，请更换SDK或者使用低版本fbx资源。SDK = %d-%d-%d, fbx = %d-%d-%d", SDKVersion.Major, SDKVersion.Minor, SDKVersion.Revision, FbxAssetsVersion.Major, FbxAssetsVersion.Minor, FbxAssetsVersion.Revision);
			return;
		}

		// ------------------------头文件信息读取完毕-------------------------------------------------

		// ------------------------fbx实体数据信息读取开始-------------------------------------------------

		// 将数据导出到场景中
		if (fbxImporter->Import(scene))
		{
			// 判断状态
			if (fbxImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
			{
				ENGINE_LOG_ERROR("数据导入场景失败，密码错误。%s", fbxImporter->GetFileName().Buffer());
				// TODO 针对加密文件，可以另行开发让用户重新输入密码的功能
				return;
			}

			// 自此，资源已经成功导入到场景中，导入器可以销毁了
			ENGINE_LOG_SUCCESS("FBX资源导入成功: %s", fbxImporter->GetFileName().Buffer());
			fbxImporter->Destroy();
			
		}
		else
		{
			ENGINE_LOG_ERROR("数据导入场景失败。%s", fbxImporter->GetFileName().Buffer());
			return;
		}


	}
}
