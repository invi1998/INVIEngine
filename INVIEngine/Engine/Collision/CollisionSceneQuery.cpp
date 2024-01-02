#include "EngineMinimal.h"
#include "CollisionSceneQuery.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"

bool FCollisionSceneQuery::RayCastSingleQuery(CWorld* world, const XMFLOAT3& origin, const XMFLOAT3& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult)
{
	// 遍历FGeometry::RenderingDataPoolVector，找到所有的mesh
	for (auto& renderData : FGeometry::RenderingDataPoolVector)
	{
		float time = 0.f;
		// 射线是否和AABB包围盒相交
		if (renderData->Bounds.intersects(origin, direction, time))
		{
			// 射线是否和模型相交
			if (renderData->Mesh->Intersect(origin, direction, time, viewInverseMatrix))
			{
				OutHitResult.HitObject = renderData->Mesh->GetOwner();
				OutHitResult.HitTime = time;
				return true;
			}
		}
	}

	return false;
}
