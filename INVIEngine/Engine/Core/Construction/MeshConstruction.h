#pragma once
#include "Mesh/Core/MeshType.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/RenderingData.h"
#include "Mesh/Core/MeshManager.h"

namespace MeshConstruction
{
	template<class T, typename ...ParamTypes>
	T* MeshComponentConstruction(CMeshManager* manage, T* mesh, ParamTypes &&...Params)
	{
		if (manage && mesh)
		{

			size_t HashKey = 0;
			mesh->BuildKey(HashKey, std::forward<ParamTypes>(Params)...);

			FRenderingData RenderingData;
			if (manage->GetRenderingPipeline().FindMeshRenderingDataByHash(HashKey, RenderingData, mesh->GetRenderLayerType()))
			{
				manage->GetRenderingPipeline().DuplicateMesh(mesh, RenderingData);
			}
			else
			{
				// ��ȡģ����Դ
				FMeshRenderingData MeshData;
				mesh->CreateMesh(MeshData, std::forward<ParamTypes>(Params)...);

				mesh->BeginInit();

				// ����mesh
				// BuildMesh(&MeshData);
				manage->GetRenderingPipeline().BuildMesh(HashKey, mesh, MeshData);

			}

			mesh->Init();


			return mesh;
		}

		return nullptr;
	}

	template<class T, typename ...ParamTypes>
	T* CreateMeshComponent(CMeshManager* manage, ParamTypes &&...Params)
	{
		if (manage)
		{
			T* MyMesh = CreateObject<T>(new T());

			return MeshComponentConstruction<T>(manage, MyMesh, Params...);
		}

		return nullptr;
	}


	

}





