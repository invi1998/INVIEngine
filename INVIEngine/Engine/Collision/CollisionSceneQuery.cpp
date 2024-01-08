#include "EngineMinimal.h"
#include "CollisionSceneQuery.h"

#include "Actor/Core/ActorObject.h"
#include "Component/Mesh/Core/MeshComponent.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"

bool FCollisionSceneQuery::RayCastSingleQuery(CWorld* world, const XMVECTOR& origin, const XMVECTOR& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult)
{
	float finalTime = FLT_MAX;

	// ����FGeometry::RenderingDataPoolVector���ҵ����е�mesh
	for (auto& renderData : FGeometry::RenderingDataPoolVector)
	{
		// �ж�mesh�Ƿ�ɱ�ѡȡ
		if (renderData->Mesh && !renderData->Mesh->IsPickUp())
		{
			continue;
		}

		// ��ȡģ�͵��������
		XMMATRIX worldMatrix = XMLoadFloat4x4(&renderData->WorldMatrix);
		XMVECTOR worldDet = XMMatrixDeterminant(worldMatrix);
		// ����
		XMMATRIX worldInverseMatrix = XMMatrixInverse(&worldDet, worldMatrix);
		// �����ߴ��ӿڿռ�ת����ģ�Ϳռ�ı仯���� (viewInverseMatrix * worldInverseMatrix) �ӿ������ * ģ�͵��������Ϊ���ǵ����������ӿڿռ��µģ�����Ҫת����ģ�Ϳռ��£���Ҫ��ת��������ռ䣬Ȼ����ת����ģ�Ϳռ�
		XMMATRIX viewWorldInverseMatrix = XMMatrixMultiply(viewInverseMatrix, worldInverseMatrix);
		// ������ת����ģ�Ϳռ�

		XMVECTOR originPointVW = XMVector3TransformCoord(origin, viewWorldInverseMatrix);	// ģ�Ϳռ��µ�����ԭ������
		XMVECTOR directionVW = XMVector3TransformNormal(direction, viewWorldInverseMatrix);		// ģ�Ϳռ��µ����߷���

		// ��λ��
		directionVW = XMVector3Normalize(directionVW);


		float boundTime = 0.f;
		// �����Ƿ��AABB��Χ���ཻ������Ĳ�����ģ�Ϳռ��µ����ߣ�������ģ�Ϳռ����������ཻ��⣩
		if (renderData->Bounds.Intersects(originPointVW, directionVW, boundTime))
		{
			// ����ཻʱ��С��0����ô��˵�����ߺ�ģ�͵�AABB��Χ�в��ཻ
			if (boundTime <= 0.f)
			{
				continue;
			}

			if (boundTime < finalTime)	// �����Χ���ཻʱ��С��֮ǰ���ཻʱ�䣬��ô��˵�����ߺ�ģ�͵�AABB��Χ���ཻ
			{
				// �����Ƿ��ģ���ཻ���������ཻ��⣩
				if (renderData->MeshRenderingData)
				{
					// ��ȡģ�������ε�����
					UINT triangleCount = renderData->IndexSize / 3;
					// �������е�������

					float triangleTime = FLT_MAX;
					for (UINT i = 0; i < triangleCount; i++)
					{
						// ��ȡ�����ε���������
						XMVECTOR v0 = XMLoadFloat3(&renderData->MeshRenderingData->VertexData[renderData->MeshRenderingData->IndexData[renderData->IndexOffsetPosition + i * 3]].Position);
						XMVECTOR v1 = XMLoadFloat3(&renderData->MeshRenderingData->VertexData[renderData->MeshRenderingData->IndexData[renderData->IndexOffsetPosition + i * 3 + 1]].Position);
						XMVECTOR v2 = XMLoadFloat3(&renderData->MeshRenderingData->VertexData[renderData->MeshRenderingData->IndexData[renderData->IndexOffsetPosition + i * 3 + 2]].Position);

						float triangleOffsetTime = 0.f;
						// �����Ƿ���������ཻ
						if (TriangleTests::Intersects(originPointVW, directionVW, v0, v1, v2, triangleOffsetTime))
						{
							finalTime = boundTime;	// �����ཻʱ��

							if (triangleOffsetTime < triangleTime)	// ����ཻʱ��С��֮ǰ���ཻʱ�䣬��ô��˵�����ߺ�ģ�͵��������ཻ�����������ཻ���
							{
								triangleTime = triangleOffsetTime;	// �����������ཻʱ��

								// �������ߺ��������ཻ�ĵ�
								XMVECTOR hitPoint = XMVectorAdd(originPointVW, XMVectorScale(directionVW, triangleTime));
								// ���ཻ��ת��������ռ�
								hitPoint = XMVector3TransformCoord(hitPoint, worldMatrix);

								// �����ཻ�㵽����ԭ��ľ���
								float distance = XMVectorGetX(XMVector3Length(XMVectorSubtract(hitPoint, origin)));

								// �������е�ķ���

								OutHitResult.HitDistance = distance;
								XMStoreFloat3(&OutHitResult.HitPoint, hitPoint);
								XMStoreFloat3(&OutHitResult.HitNormal, XMVector3Normalize(XMVector3Cross(XMVectorSubtract(v1, v0), XMVectorSubtract(v2, v0))));
								OutHitResult.bHit = true;
								OutHitResult.HitComponent = renderData->Mesh;
								OutHitResult.Time = triangleOffsetTime;
								OutHitResult.HitRenderingData = renderData;
								if (renderData->Mesh)
								{
									OutHitResult.HitActor = dynamic_cast<GActorObject*>(renderData->Mesh->GetOwner());
								}

								return true;


								// ����ཻ�㵽����ԭ��ľ���С��֮ǰ���ཻ�㵽����ԭ��ľ��룬��ô�͸����ཻ��
								/*if (distance < OutHitResult.HitDistance)
								{
									OutHitResult.HitDistance = distance;
									XMStoreFloat3(&OutHitResult.HitPoint, hitPoint);
									XMStoreFloat3(&OutHitResult.HitNormal, XMVector3Normalize(XMVector3Cross(XMVectorSubtract(v1, v0), XMVectorSubtract(v2, v0))));
									OutHitResult.bHit = true;
									OutHitResult.HitComponent = renderData->Mesh;
									OutHitResult.Time = triangleOffsetTime;
									return true;
								}*/
							}
						}
					}
				}
			}
		}
	}

	return false;
}
