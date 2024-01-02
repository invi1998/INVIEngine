#include "EngineMinimal.h"
#include "CollisionSceneQuery.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"

bool FCollisionSceneQuery::RayCastSingleQuery(CWorld* world, const XMFLOAT3& origin, const XMFLOAT3& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult)
{
	// ����FGeometry::RenderingDataPoolVector���ҵ����е�mesh
	for (auto& renderData : FGeometry::RenderingDataPoolVector)
	{
		XMVECTOR OriginPoint = XMLoadFloat3(&origin);

		XMVECTOR Direction = XMLoadFloat3(&direction);

		// ��ȡģ�͵��������
		XMMATRIX worldMatrix = XMLoadFloat4x4(&renderData->WorldMatrix);
		XMVECTOR worldDet = XMMatrixDeterminant(worldMatrix);
		// ����
		XMMATRIX worldInverseMatrix = XMMatrixInverse(&worldDet, worldMatrix);
		// �����ߴ��ӿڿռ�ת����ģ�Ϳռ�ı仯���� (viewInverseMatrix * worldInverseMatrix) �ӿ������ * ģ�͵��������Ϊ���ǵ����������ӿڿռ��µģ�����Ҫת����ģ�Ϳռ��£���Ҫ��ת��������ռ䣬Ȼ����ת����ģ�Ϳռ�
		XMMATRIX viewWorldInverseMatrix = XMMatrixMultiply(viewInverseMatrix, worldInverseMatrix);
		// ������ת����ģ�Ϳռ�

		XMVECTOR originPointVW = XMVector3TransformCoord(OriginPoint, viewWorldInverseMatrix);	// ģ�Ϳռ��µ�����ԭ������
		XMVECTOR directionVW = XMVector3TransformNormal(Direction, viewWorldInverseMatrix);		// ģ�Ϳռ��µ����߷���


		float time = 0.f;
		// �����Ƿ��AABB��Χ���ཻ������Ĳ�����ģ�Ϳռ��µ����ߣ�������ģ�Ϳռ����������ཻ��⣩
		if (renderData->Bounds.Intersects(originPointVW, directionVW, time))
		{
			// �����Ƿ��ģ���ཻ���������ཻ��⣩
			if (renderData->MeshRenderingData)
			{
				// ��ȡģ�������ε�����
				UINT triangleCount = renderData->IndexSize / 3;
				// �������е�������
				for (UINT i = 0; i < triangleCount; i++)
				{
					// ��ȡ�����ε���������
					XMVECTOR v0 = XMLoadFloat3(&renderData->MeshRenderingData->VertexData[renderData->MeshRenderingData->IndexData[renderData->IndexOffsetPosition + i * 3]].Position);
					XMVECTOR v1 = XMLoadFloat3(&renderData->MeshRenderingData->VertexData[renderData->MeshRenderingData->IndexData[renderData->IndexOffsetPosition + i * 3 + 1]].Position);
					XMVECTOR v2 = XMLoadFloat3(&renderData->MeshRenderingData->VertexData[renderData->MeshRenderingData->IndexData[renderData->IndexOffsetPosition + i * 3 + 2]].Position);

					float triangleTime = 0.f;
					// �����Ƿ���������ཻ
					if (TriangleTests::Intersects(originPointVW, directionVW, v0, v1, v2, triangleTime))
					{
												// �������ߺ��������ཻ�ĵ�
						XMVECTOR hitPoint = XMVectorAdd(originPointVW, XMVectorScale(directionVW, triangleTime));
						// ���ཻ��ת��������ռ�
						hitPoint = XMVector3TransformCoord(hitPoint, worldMatrix);

						// �����ཻ�㵽����ԭ��ľ���
						float distance = XMVectorGetX(XMVector3Length(XMVectorSubtract(hitPoint, OriginPoint)));

						// ����ཻ�㵽����ԭ��ľ���С��֮ǰ���ཻ�㵽����ԭ��ľ��룬��ô�͸����ཻ��
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
