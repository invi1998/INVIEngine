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

	// XML
	if (FbxNode* Node = fbxScene->GetRootNode())
	{
		// 拿到场景，然后从场景的根节点开始遍历
		for (int i = 0; i < Node->GetChildCount(); i++)
		{
			RecursiveLoadMesh(Node->GetChild(i), outData);
		}
	}
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

void CFBXAssetImport::RecursiveLoadMesh(FbxNode* node, FFBXRenderData& outData)
{
	// XML
	if (node->GetNodeAttribute() == nullptr)
	{
		// 空节点
		return;
	}
	else
	{
		// 获取属性类型（标志，骨骼，模型，相机，灯光，样条等等。。）
		FbxNodeAttribute::EType attributeType = node->GetNodeAttribute()->GetAttributeType();

		// 现在只关心mesh
		if (attributeType == FbxNodeAttribute::eMesh)
		{
			// Mesh数据
			GetMesh(node, outData);
		}
	}
}

void CFBXAssetImport::GetMesh(FbxNode* node, FFBXRenderData& outData)
{
	// 一个网格里会有很多信息，材质，颜色，贴图等等

	FbxMesh* nodeMesh = static_cast<FbxMesh*>(node->GetNodeAttribute());

	GetPolygons(nodeMesh, outData);
}

void CFBXAssetImport::GetPolygons(FbxMesh* mesh, FFBXRenderData& outData)
{
	// 获取图元数量（这个图元可以是三角形，四边形，五边形等等，目前引擎只支持3角形）
	int polygonCount = mesh->GetPolygonCount();

	// 拿到控制点（实际图元顶点）
	FbxVector4* controlPoints = mesh->GetControlPoints();

	// 记录点的数量
	int vertexId = 0;

	// 遍历每个图元
	for (int i = 0; i < polygonCount; i++)
	{
		// 当前图元是几边形
		int polygonSize = mesh->GetPolygonSize(i);
		// 遍历点的类型
		for (int j = 0; j < polygonSize; j++)
		{
			// 拿到控制点索引
			int controlPointIndex = mesh->GetPolygonVertex(i, j);

			// 拿到点的位置
			FbxVector4 coordinates = controlPoints[controlPointIndex];

			// uv
			for (int l = 0; l < mesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* textureUV = mesh->GetElementUV(l);

				// 对于UV来说，他的类型也很多，有控制点的，有polygon顶点的
				auto modeType = textureUV->GetMappingMode();

				// 这里我们只关心polygon顶点的uv
				if (modeType == FbxLayerElement::eByPolygonVertex)
				{
					// 拿到贴图UV
					int textureUVIndex = mesh->GetTextureUVIndex(i, j);

					// 通过这个UV再去拿到引用类型
					auto referenceMode = textureUV->GetReferenceMode();

					// 为什么这里需要判断类型呢？学过图形学的都知道，对于fbx格式，它一个点存储的东西是很多，一个点是可以被多个图元共用的，比如法线信息，一个点可能就会存储多个法线信息

					// 判断当前uv的引用类型是否是index
					if (referenceMode == FbxLayerElement::eIndex)
					{
						// 获取UV
						FbxVector2 UV = textureUV->GetDirectArray().GetAt(textureUVIndex);
					}
				}
			}

			// 法线
			// 遍历当前点的法线数量
			for (int k = 0; k < mesh->GetElementNormalCount(); ++k)
			{
				// 拿到当前k索引的法线
				FbxGeometryElementNormal* normal = mesh->GetElementNormal(k);

				// 同理，法线的类型也很多，但是这里我们只关注polygon顶点法线
				if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					// 判断引用关系
					switch (normal->GetReferenceMode())
					{
						case FbxLayerElement::eDirect:
						{
							FbxVector4 directNormal = normal->GetDirectArray().GetAt(vertexId);
							break;
						};
						case FbxLayerElement::eIndex: break;
						case FbxLayerElement::eIndexToDirect: break;
						default: break;
					}
				}

			}


			// 切线 T
			// 遍历当前点的切线数量
			for (int k = 0; k < mesh->GetElementTangentCount(); ++k)
			{
				// 更具切线索引k拿到切线
				FbxGeometryElementTangent* tangent = mesh->GetElementTangent(k);

				// 同理，切线的类型也很多，但是这里我们只关注polygon顶点切线
				if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (tangent->GetReferenceMode())
					{
						case FbxLayerElement::eDirect:
						{
							const int id = tangent->GetIndexArray().GetAt(vertexId);
							FbxVector4 directTangent = tangent->GetDirectArray().GetAt(id);
							break;
						};
						case FbxLayerElement::eIndex: break;
						case FbxLayerElement::eIndexToDirect: break;
						default: break;
					}
				}

			}

			// 切线B （fbx里已经有计算好的切线B，但是我引擎里，切线B是自己在shader中计算的(法线和切线T做叉乘)，所以这里不读也可以）
			for (int k = 0; k < mesh->GetElementBinormalCount(); ++k)
			{
				// 更具切线索引k拿到切线
				FbxGeometryElementBinormal* binormal = mesh->GetElementBinormal(k);

				// 同理，切线的类型也很多，但是这里我们只关注polygon顶点切线
				if (binormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (binormal->GetReferenceMode())
					{
					case FbxLayerElement::eDirect:
					{
						FbxVector4 directBinormal = binormal->GetDirectArray().GetAt(vertexId);
						break;
					};
					case FbxLayerElement::eIndex: break;
					case FbxLayerElement::eIndexToDirect: break;
					default: break;
					}
				}
			}


			vertexId++;
		}
	}

}


