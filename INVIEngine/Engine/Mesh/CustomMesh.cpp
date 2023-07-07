#include "CustomMesh.h"

void CCustomMesh::BeginInit()
{
	CMesh::BeginInit();
}

void CCustomMesh::Tick(float DeltaTime)
{
	CMesh::Tick(DeltaTime);
}

CCustomMesh::~CCustomMesh()
{
}

void CCustomMesh::Init()
{
	CMesh::Init();
}

void CCustomMesh::PreDraw(float DeltaTime)
{
	CMesh::PreDraw(DeltaTime);
}

void CCustomMesh::Draw(float DeltaTime)
{
	CMesh::Draw(DeltaTime);
}

void CCustomMesh::PostDraw(float DeltaTime)
{
	CMesh::PostDraw(DeltaTime);
}

void CCustomMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{
	CMesh::BuildMesh(InRenderingData);
}

CCustomMesh* CCustomMesh::CreateMesh(const std::string& InPath)
{
	FMeshRendingData MeshData;

	{
		// ��ȡ�ļ���С
		unsigned int FileSize = get_file_size_by_filename(InPath.c_str());
		// �����ļ���С����Buffer
		char* Buffer = new char[FileSize + 1];
		memset(Buffer, 0, FileSize + 1);
		// ���ļ����ݶ�ȡ��buffer
		get_file_buf(InPath.c_str(), Buffer);

		if (!LoadObjFormBuffer(Buffer, FileSize, MeshData))
		{
			ENGINE_LOG_ERROR("ģ�͵���ʧ��");
		}

		delete Buffer;
	}

	CCustomMesh* CustomMesh = new CCustomMesh();
	CustomMesh->BuildMesh(&MeshData);

	CustomMesh->Init();

	return CustomMesh;
}

bool CCustomMesh::LoadObjFormBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRendingData& MeshData)
{
	if (InBufferSize > 0)
	{
		std::stringstream BufferStream(InBuffer);
		char TempLine[256] = { 0 };
		std::string MidTemp;
		while (!BufferStream.eof())
		{
			memset(TempLine, 0, 256);

			// ��ȡһ��
			BufferStream.getline(TempLine, 256);
			if (strlen(TempLine) > 0)
			{
				if (TempLine[0] == 'v')
				{
					std::stringstream LineStream(TempLine);
					LineStream >> MidTemp;

					// ��ͼ
					if (TempLine[1] == 't')
					{
						
					}
					// ����
					else if (TempLine[1] == 'n')
					{

					}
					// ����
					else
					{
						MeshData.VertexData.push_back(FVertex(XMFLOAT3(), XMFLOAT4(Colors::Azure)));

						int TopIndex = MeshData.VertexData.size() - 1;
						XMFLOAT3& InPos = MeshData.VertexData[TopIndex].Position;
						// ����λ��
						LineStream >> InPos.x;
						LineStream >> InPos.y;
						LineStream >> InPos.z;
					}
				}
				else if (TempLine[0] == 'f')
				{
					// ����ָ��
					// ���ݸ�ʽ ��������/��������/��������
					std::stringstream LineStream(TempLine);

					LineStream >> MidTemp;

					char SaveLineString[256] = { 0 };
					char TempBuffer[256] = { 0 };
					for (size_t i = 0; i < 3; i++)
					{
						memset(SaveLineString, 0, 256);

						LineStream >> SaveLineString;
						// �ҵ�����λ��
						int StringPosA = find_string(SaveLineString, "/", 0);
						memset(TempBuffer, 0, 256);
						char* VPosIndex = string_mid(SaveLineString, TempBuffer, 0, StringPosA);

						// �ŵ�����������
						MeshData.IndexData.push_back(atoi(VPosIndex));

						// Ѱ����������λ��
						int StringPosB = find_string(SaveLineString, "/", StringPosA + 1);
						memset(TempBuffer, 0, 256);
						char* TexCoordIndex = string_mid(SaveLineString, TempBuffer, StringPosA + 1, StringPosB - StringPosA);

						// Ѱ�ҷ�������
						memset(TempBuffer, 0, 256);
						char* NormalIndex = string_mid(SaveLineString, TempBuffer, StringPosB + 1, strlen(SaveLineString) - (StringPosB + 1));

					}
				}
			}
		}
	}

	return true;
}
