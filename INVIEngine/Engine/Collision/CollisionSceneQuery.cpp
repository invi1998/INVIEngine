#include "EngineMinimal.h"
#include "CollisionSceneQuery.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"

bool FCollisionSceneQuery::RayCastSingleQuery(CWorld* world, const XMFLOAT3& origin, const XMFLOAT3& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult)
{
	// 遍历FGeometry::RenderingDataPoolVector，找到所有的mesh
	for (auto& renderData : FGeometry::RenderingDataPoolVector)
	{
		XMVECTOR OriginPoint = XMLoadFloat3(&origin);

		XMVECTOR Direction = XMLoadFloat3(&direction);

		// 获取模型的世界矩阵
		XMMATRIX worldMatrix = XMLoadFloat4x4(&renderData->WorldMatrix);
		// 求逆
		XMMATRIX worldInverseMatrix = XMMatrixInverse(&XMMatrixDeterminant(worldMatrix), worldMatrix);
		// 将射线从视口空间转换到模型空间的变化矩阵 (viewInverseMatrix * worldInverseMatrix) 视口逆矩阵 * 模型的逆矩阵，因为我们的射线是在视口空间下的，所以要转换到模型空间下，就要先转换到世界空间，然后再转换到模型空间
		XMMATRIX viewWorldInverseMatrix = XMMatrixMultiply(viewInverseMatrix, worldInverseMatrix);
		// 将射线转换到模型空间

		XMVECTOR originPointVW = XMVector3TransformCoord(OriginPoint, viewWorldInverseMatrix);	// 模型空间下的射线原点坐标
		XMVECTOR directionVW = XMVector3TransformNormal(Direction, viewWorldInverseMatrix);		// 模型空间下的射线方向


		float time = 0.f;
		// 射线是否和AABB包围盒相交（传入的参数是模型空间下的射线，我们在模型空间下做射线相交检测）
		if (renderData->Bounds.Intersects(originPointVW, directionVW, time))
		{
			// 射线是否和模型相交

		}
	}

	return false;
}
