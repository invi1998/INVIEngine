#include "EngineMinimal.h"
#include "FBXSdk.h"

CFBXAssetImport::CFBXAssetImport()
{
	InitializeSDKObjects();
}

CFBXAssetImport::~CFBXAssetImport()
{
	if (fbxManager)
	{
		fbxManager->Destroy();
	}
}

void CFBXAssetImport::LoadMeshData(const std::string& path, FMeshRenderingData& MeshData)
{
	// FbxString fbxPath(path.c_str());
	// LoadScene(fbxScene, fbxPath.Buffer());

	LoadScene(fbxScene, path.c_str());

	// XML
	if (FbxNode* Node = fbxScene->GetRootNode())
	{
		// �õ�������Ȼ��ӳ����ĸ��ڵ㿪ʼ����
		for (int i = 0; i < Node->GetChildCount(); i++)
		{
			RecursiveLoadMesh(Node->GetChild(i), MeshData);
		}
	}
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

void CFBXAssetImport::RecursiveLoadMesh(FbxNode* node, FFBXRenderData& MeshData)
{
	// XML
	if (node->GetNodeAttribute() == nullptr)
	{
		// �սڵ�
		return;
	}
	else
	{
		// ��ȡ�������ͣ���־��������ģ�ͣ�������ƹ⣬�����ȵȡ�����
		FbxNodeAttribute::EType attributeType = node->GetNodeAttribute()->GetAttributeType();

		// ����ֻ����mesh
		if (attributeType == FbxNodeAttribute::eMesh)
		{
			MeshData.ModelData.push_back(FFBXModel());
			
			// Mesh����
			GetMesh(node, MeshData.ModelData[MeshData.ModelData.size() - 1]);
		}
	}
}

void CFBXAssetImport::GetMesh(FbxNode* node, FFBXModel& ModelData)
{
	// һ����������кܶ���Ϣ�����ʣ���ɫ����ͼ�ȵ�

	FbxMesh* nodeMesh = static_cast<FbxMesh*>(node->GetNodeAttribute());

	ModelData.MeshData.push_back(FFBXMesh());
	

	// ��������ֻ���Ķ������Ϣ
	GetPolygons(nodeMesh, ModelData.MeshData[ModelData.MeshData.size() - 1]);

	// index
	GetIndex(ModelData.MeshData[ModelData.MeshData.size() - 1]);
}

void CFBXAssetImport::GetPolygons(FbxMesh* mesh, FFBXMesh& MeshData)
{
	// ��ȡͼԪ���������ͼԪ�����������Σ��ı��Σ�����εȵȣ�Ŀǰ����ֻ֧��3���Σ�
	int polygonCount = mesh->GetPolygonCount();

	// �õ����Ƶ㣨ʵ��ͼԪ���㣩
	FbxVector4* controlPoints = mesh->GetControlPoints();

	// ��¼�������
	int vertexId = 0;

	std::vector<XMFLOAT3> Position{};	// ��������
	std::vector<XMFLOAT3> Normal{};		// ���㷨��
	std::vector<XMFLOAT3> UTangent{};	// ��������
	std::vector<XMFLOAT2> TexCoord{};	// ��������

	// ����ÿ��ͼԪ
	for (int i = 0; i < polygonCount; i++)
	{

		MeshData.VertexData.push_back(FFBXTrianglePolygon());
		FFBXTrianglePolygon& triangle = MeshData.VertexData[MeshData.VertexData.size() - 1];

		// ��ǰͼԪ�Ǽ�����
		int polygonSize = mesh->GetPolygonSize(i);
		// �����������
		for (int j = 0; j < polygonSize; j++)
		{
			// �õ����Ƶ�����
			int controlPointIndex = mesh->GetPolygonVertex(i, j);

			// �õ����λ��
			const FbxVector4 coordinates = controlPoints[controlPointIndex];

			// ������Ϣ
			FbxDouble3 scale = mesh->GetNode()->LclScaling;


			triangle.Vertexs[j].Position.x = coordinates.mData[0] * scale.mData[0];
			triangle.Vertexs[j].Position.y = coordinates.mData[1] * scale.mData[1];
			triangle.Vertexs[j].Position.z = -coordinates.mData[2] * scale.mData[2];

			// uv
			for (int l = 0; l < mesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* textureUV = mesh->GetElementUV(l);

				// ����UV��˵����������Ҳ�ܶ࣬�п��Ƶ�ģ���polygon�����
				auto modeType = textureUV->GetMappingMode();

				// ͨ�����UV��ȥ�õ���������
				auto referenceMode = textureUV->GetReferenceMode();

				// ��������ֻ����polygon�����uv
				if (modeType == FbxLayerElement::eByPolygonVertex)
				{
					// Ϊʲô������Ҫ�ж������أ�ѧ��ͼ��ѧ�Ķ�֪��������fbx��ʽ����һ����洢�Ķ����Ǻܶ࣬һ�����ǿ��Ա����ͼԪ���õģ����編����Ϣ��һ������ܾͻ�洢���������Ϣ

					// �жϵ�ǰuv�����������Ƿ���eDirect
					if (referenceMode == FbxLayerElement::eIndexToDirect)
					{
						const FbxVector2 UV = textureUV->GetDirectArray().GetAt(controlPointIndex);

						triangle.Vertexs[j].UV.x = UV.mData[0];
						triangle.Vertexs[j].UV.y = 1.0f - UV.mData[1];	// V�Ƿ��ģ��� OpenGL��һ�� ������ϵ���⣩
					}
				}
				// ����ǿ��Ƶ�����
				else if (modeType == FbxLayerElement::eByControlPoint)
				{
					switch (referenceMode)
					{
					case FbxLayerElement::eDirect:
						{
							// ��ȡUV
							const FbxVector2 UV = textureUV->GetDirectArray().GetAt(controlPointIndex);

							triangle.Vertexs[j].UV.x = UV.mData[0];
							triangle.Vertexs[j].UV.y = 1.0f - UV.mData[1];	// V�Ƿ��ģ��� OpenGL��һ�� ������ϵ���⣩
							break;
						};
					case FbxLayerElement::eIndex: break;
					case FbxLayerElement::eIndexToDirect:
						{
							int id = textureUV->GetIndexArray().GetAt(controlPointIndex);
							const FbxVector2 UV = textureUV->GetDirectArray().GetAt(id);

							triangle.Vertexs[j].UV.x = UV.mData[0];
							triangle.Vertexs[j].UV.y = 1.0f - UV.mData[1];	// V�Ƿ��ģ��� OpenGL��һ�� ������ϵ���⣩
							break;
						};
					default: break;
					}
				}
			}

			// ����
			// ������ǰ��ķ�������
			for (int k = 0; k < mesh->GetElementNormalCount(); ++k)
			{
				// �õ���ǰk�����ķ���
				FbxGeometryElementNormal* normal = mesh->GetElementNormal(k);

				// ͬ�����ߵ�����Ҳ�ܶ࣬������������ֻ��עpolygon���㷨��
				if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					// �ж����ù�ϵ
					switch (normal->GetReferenceMode())
					{
						case FbxLayerElement::eDirect:
						{
							FbxVector4 directNormal = normal->GetDirectArray().GetAt(vertexId);

							triangle.Vertexs[j].Normal.x = directNormal.mData[0];
							triangle.Vertexs[j].Normal.y = directNormal.mData[1];
							triangle.Vertexs[j].Normal.z = directNormal.mData[2];
							break;
						};
						case FbxLayerElement::eIndex: break;
						case FbxLayerElement::eIndexToDirect:
						{
							int id = normal->GetIndexArray().GetAt(vertexId);
							FbxVector4 directNormal = normal->GetDirectArray().GetAt(id);

							triangle.Vertexs[j].Normal.x = directNormal.mData[0];
							triangle.Vertexs[j].Normal.y = directNormal.mData[1];
							triangle.Vertexs[j].Normal.z = directNormal.mData[2];
							break;
						};
						default: break;
					}
				}
				else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					// �ж����ù�ϵ
					switch (normal->GetReferenceMode())
					{
					case FbxLayerElement::eDirect:
					{
						FbxVector4 directNormal = normal->GetDirectArray().GetAt(controlPointIndex);

						triangle.Vertexs[j].Normal.x = directNormal.mData[0];
						triangle.Vertexs[j].Normal.y = directNormal.mData[1];
						triangle.Vertexs[j].Normal.z = directNormal.mData[2];
						break;
					};
					case FbxLayerElement::eIndex: break;
					case FbxLayerElement::eIndexToDirect:
					{
						int id = normal->GetIndexArray().GetAt(controlPointIndex);
						FbxVector4 directNormal = normal->GetDirectArray().GetAt(id);

						triangle.Vertexs[j].Normal.x = directNormal.mData[0];
						triangle.Vertexs[j].Normal.y = directNormal.mData[1];
						triangle.Vertexs[j].Normal.z = directNormal.mData[2];
						break;
					};
					default: break;
					}
				}

			}


			// ���� T
			// ������ǰ�����������
			for (int k = 0; k < mesh->GetElementTangentCount(); ++k)
			{
				// ������������k�õ�����
				FbxGeometryElementTangent* tangent = mesh->GetElementTangent(k);

				// ͬ�����ߵ�����Ҳ�ܶ࣬������������ֻ��עpolygon��������
				if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (tangent->GetReferenceMode())
					{
						case FbxLayerElement::eDirect:
						{
							const int id = tangent->GetIndexArray().GetAt(vertexId);
							FbxVector4 directTangent = tangent->GetDirectArray().GetAt(id);

							triangle.Vertexs[j].Tangent.x = directTangent.mData[0];
							triangle.Vertexs[j].Tangent.y = directTangent.mData[1];
							triangle.Vertexs[j].Tangent.z = directTangent.mData[2];
							break;
						};
						case FbxLayerElement::eIndex: break;
						case FbxLayerElement::eIndexToDirect:
							{
								int id = tangent->GetIndexArray().GetAt(vertexId);
								FbxVector4 directTangent = tangent->GetDirectArray().GetAt(id);

								triangle.Vertexs[j].Tangent.x = directTangent.mData[0];
								triangle.Vertexs[j].Tangent.y = directTangent.mData[1];
								triangle.Vertexs[j].Tangent.z = directTangent.mData[2];

								break;
							};
						default: break;
					}
				}

			}

			// ����B ��fbx���Ѿ��м���õ�����B�����������������B���Լ���shader�м����(���ߺ�����T�����)���������ﲻ��Ҳ���ԣ�
			for (int k = 0; k < mesh->GetElementBinormalCount(); ++k)
			{
				// ������������k�õ�����
				FbxGeometryElementBinormal* binormal = mesh->GetElementBinormal(k);

				// ͬ�����ߵ�����Ҳ�ܶ࣬������������ֻ��עpolygon��������
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

void CFBXAssetImport::GetMaterial()
{
}

void CFBXAssetImport::GetIndex(FFBXMesh& mesh)
{
	uint16_t length = mesh.VertexData.size() * 3;
	mesh.IndexData.resize(length);

	for (uint16_t i = 0; i < length; i++)
	{
		mesh.IndexData[i] = i;
	}
}


