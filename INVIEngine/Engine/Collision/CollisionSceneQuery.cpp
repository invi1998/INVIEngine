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
		// ����
		XMMATRIX worldInverseMatrix = XMMatrixInverse(&XMMatrixDeterminant(worldMatrix), worldMatrix);
		// �����ߴ��ӿڿռ�ת����ģ�Ϳռ�ı仯���� (viewInverseMatrix * worldInverseMatrix) �ӿ������ * ģ�͵��������Ϊ���ǵ����������ӿڿռ��µģ�����Ҫת����ģ�Ϳռ��£���Ҫ��ת��������ռ䣬Ȼ����ת����ģ�Ϳռ�
		XMMATRIX viewWorldInverseMatrix = XMMatrixMultiply(viewInverseMatrix, worldInverseMatrix);
		// ������ת����ģ�Ϳռ�

		XMVECTOR originPointVW = XMVector3TransformCoord(OriginPoint, viewWorldInverseMatrix);	// ģ�Ϳռ��µ�����ԭ������
		XMVECTOR directionVW = XMVector3TransformNormal(Direction, viewWorldInverseMatrix);		// ģ�Ϳռ��µ����߷���


		float time = 0.f;
		// �����Ƿ��AABB��Χ���ཻ������Ĳ�����ģ�Ϳռ��µ����ߣ�������ģ�Ϳռ����������ཻ��⣩
		if (renderData->Bounds.Intersects(originPointVW, directionVW, time))
		{
			// �����Ƿ��ģ���ཻ

		}
	}

	return false;
}
