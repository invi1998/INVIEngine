#pragma once

#include "Core/Construction/MeshConstruction.h"
#include "Mesh/Core/Mesh.h"

namespace ActorMeshConstruction
{
	template<class T, typename ...ParamTypes>
	T* CreateMeshRenderData(CMeshManager* meshManage, GMesh* mesh, ParamTypes &&...Params)
	{
		return MeshConstruction::MeshComponentConstruction<T>(meshManage, mesh->GetMeshComponent<T>(), Params...);
	}

}



