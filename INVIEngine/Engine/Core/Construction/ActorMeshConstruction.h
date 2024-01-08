#pragma once

#include "Core/Construction/MeshConstruction.h"
#include "Mesh/Core/Mesh.h"

namespace ActorMeshConstruction
{
	template<class T, typename ...ParamTypes>
	T* CreateMeshRenderData(CMeshManager* meshManage, GMesh* mesh, ParamTypes &&...Params)
	{
		return MeshConstruction::CreateMeshComponent<T>(meshManage, mesh->GetMeshComponent<T>(), Params...);
	}

	// 传入组件，创建一个MeshRenderData
	template<class T, typename ...ParamTypes>
	T* CreateMeshRenderDataByComponent(CMeshManager* meshManage, T* meshComponent, ParamTypes &&...Params)
	{
		return MeshConstruction::CreateMeshComponent<T>(meshManage, meshComponent, Params...);
	}

}



