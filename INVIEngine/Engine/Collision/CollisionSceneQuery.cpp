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
		XMVECTOR worldDet = XMMatrixDeterminant(worldMatrix);
		// 求逆
		XMMATRIX worldInverseMatrix = XMMatrixInverse(&worldDet, worldMatrix);
		// 将射线从视口空间转换到模型空间的变化矩阵 (viewInverseMatrix * worldInverseMatrix) 视口逆矩阵 * 模型的逆矩阵，因为我们的射线是在视口空间下的，所以要转换到模型空间下，就要先转换到世界空间，然后再转换到模型空间
		XMMATRIX viewWorldInverseMatrix = XMMatrixMultiply(viewInverseMatrix, worldInverseMatrix);
		// 将射线转换到模型空间

		XMVECTOR originPointVW = XMVector3TransformCoord(OriginPoint, viewWorldInverseMatrix);	// 模型空间下的射线原点坐标
		XMVECTOR directionVW = XMVector3TransformNormal(Direction, viewWorldInverseMatrix);		// 模型空间下的射线方向


		float time = 0.f;
		// 射线是否和AABB包围盒相交（传入的参数是模型空间下的射线，我们在模型空间下做射线相交检测）
		if (renderData->Bounds.Intersects(originPointVW, directionVW, time))
		{
			// 射线是否和模型相交（三角形相交检测）
			if (renderData->MeshRenderingData)
			{
				// 获取模型三角形的数量
				UINT triangleCount = renderData->IndexSize / 3;
				// 遍历所有的三角形
				for (UINT i = 0; i < triangleCount; i++)
				{
					// 获取三角形的三个顶点
					XMVECTOR v0 = XMLoadFloat3(&renderData->MeshRenderingData->VertexData[renderData->MeshRenderingData->IndexData[renderData->IndexOffsetPosition + i * 3]].Position);
					XMVECTOR v1 = XMLoadFloat3(&renderData->MeshRenderingData->VertexData[renderData->MeshRenderingData->IndexData[renderData->IndexOffsetPosition + i * 3 + 1]].Position);
					XMVECTOR v2 = XMLoadFloat3(&renderData->MeshRenderingData->VertexData[renderData->MeshRenderingData->IndexData[renderData->IndexOffsetPosition + i * 3 + 2]].Position);

					float triangleTime = 0.f;
					// 射线是否和三角形相交
					if (TriangleTests::Intersects(originPointVW, directionVW, v0, v1, v2, triangleTime))
					{
												// 计算射线和三角形相交的点
						XMVECTOR hitPoint = XMVectorAdd(originPointVW, XMVectorScale(directionVW, triangleTime));
						// 将相交点转换到世界空间
						hitPoint = XMVector3TransformCoord(hitPoint, worldMatrix);

						// 计算相交点到射线原点的距离
						float distance = XMVectorGetX(XMVector3Length(XMVectorSubtract(hitPoint, OriginPoint)));

						// 如果相交点到射线原点的距离小于之前的相交点到射线原点的距离，那么就更新相交点
						if (distance < OutHitResult.Distance)
						{
							OutHitResult.Distance = distance;
							XMStoreFloat3(&OutHitResult.Location, hitPoint);
							XMStoreFloat3(&OutHitResult.Normal, XMVector3Normalize(XMVector3Cross(XMVectorSubtract(v1, v0), XMVectorSubtract(v2, v0))));
							OutHitResult.bHit = true;
							// OutHitResult.HitComponent = renderData->GetComponent();
							return true;
						}
					}
				}
			}

		}
	}

	return false;
}
