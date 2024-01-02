#include "EngineMinimal.h"
#include "CollisionSceneQuery.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"

bool FCollisionSceneQuery::RayCastSingleQuery(CWorld* world, const XMFLOAT3& origin, const XMFLOAT3& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult)
{
	// ����FGeometry::RenderingDataPoolVector���ҵ����е�mesh
	for (auto& renderData : FGeometry::RenderingDataPoolVector)
	{
		float time = 0.f;
		// �����Ƿ��AABB��Χ���ཻ
		if (renderData->Bounds.intersects(origin, direction, time))
		{
			// �����Ƿ��ģ���ཻ
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
