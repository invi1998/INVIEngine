#include "EngineMinimal.h"
#include "FBXSdk.h"


void CFBXAssetImport::LoadMeshData(const std::string& path, FFBXRenderData& outData)
{
	FbxManager* fbxManager = nullptr;
	FbxScene* fbxScene = nullptr;

	InitializeSDKObjects(fbxManager, fbxScene);
}

void CFBXAssetImport::InitializeSDKObjects(FbxManager*& manager, FbxScene*& scene)
{
}
