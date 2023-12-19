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

void CFBXAssetImport::LoadScene(FbxDocument* scene, const char* fileName)
{
	// SDK�汾��Ϣ
	FFBXVersion SDKVersion{};
	FbxManager::GetFileFormatVersion(SDKVersion.Major, SDKVersion.Minor, SDKVersion.Revision);

	// ����������
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "INVI_FBX_IMPORTER");
	// ��������ʼ��
	if (fbxImporter->Initialize(fileName, -1, fbxManager->GetIOSettings()))
	{
		FFBXVersion FbxAssetsVersion{};
		fbxImporter->GetFileVersion(FbxAssetsVersion.Major, FbxAssetsVersion.Minor, FbxAssetsVersion.Revision);

		// �ж��ǲ���fbx��Դ
		if (!fbxImporter->IsFBX())
		{
			ENGINE_LOG_ERROR("��ǰ��Դ����fbx��Դ��%s", fbxImporter->GetFileName().Buffer());
			return;
		}

		// FBX��Դ��SDK�汾���бȶ�
		if (SDKVersion < FbxAssetsVersion)
		{
			ENGINE_LOG_ERROR("SDK�汾����FBX��Դ�汾�������SDK����ʹ�õͰ汾fbx��Դ��SDK = %d-%d-%d, fbx = %d-%d-%d", SDKVersion.Major, SDKVersion.Minor, SDKVersion.Revision, FbxAssetsVersion.Major, FbxAssetsVersion.Minor, FbxAssetsVersion.Revision);
			return;
		}

		// ------------------------ͷ�ļ���Ϣ��ȡ���-------------------------------------------------

		// ------------------------fbxʵ��������Ϣ��ȡ��ʼ-------------------------------------------------

		// �����ݵ�����������
		if (fbxImporter->Import(scene))
		{
			// �ж�״̬
			if (fbxImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
			{
				ENGINE_LOG_ERROR("���ݵ��볡��ʧ�ܣ��������%s", fbxImporter->GetFileName().Buffer());
				// TODO ��Լ����ļ����������п������û�������������Ĺ���
				return;
			}

			// �Դˣ���Դ�Ѿ��ɹ����뵽�����У�����������������
			ENGINE_LOG_SUCCESS("FBX��Դ����ɹ�: %s", fbxImporter->GetFileName().Buffer());
			fbxImporter->Destroy();
			
		}
		else
		{
			ENGINE_LOG_ERROR("���ݵ��볡��ʧ�ܡ�%s", fbxImporter->GetFileName().Buffer());
			return;
		}


	}
}
