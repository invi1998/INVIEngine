#include "EngineMinimal.h"
#include "RaycastSystemLibrary.h"

#include "Collision/CollisionSceneQuery.h"
#include "Config/EngineRenderConfig.h"
#include "Core/QuaternionCamera.h"
#include "Core/World.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"

bool FRayCastSystemLibrary::GetHitResultByScreen(CWorld* world, const XMFLOAT2& mousePos,
                                                 EngineType::FHitResult& OutHitResult)
{
	const GQuaternionCamera* camera = world->GetQuaternionCamera();
	if (camera)
	{
		int H = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
		int W = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;

		XMFLOAT2 View{};

		View.x = (2.f * mousePos.x / W - 1.f) / camera->GetProjectionMatrix()._11;
		View.y = (-2.f * mousePos.y / H + 1.f) / camera->GetProjectionMatrix()._22;

		// ��������ԭ�㣨��㣩�ӿ��µ����꣬��ʾ�㣬�������Ϊ1
		XMVECTOR OriginPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		// ���߷���zֵΪ1�����Ϊ0����Ϊ��һ���������������Ϊ0��Ȼ�������ڼ��㹫ʽ��ʱ�򣬼ٶ���zֵΪ1����������z��1
		XMVECTOR Direction = XMVectorSet(View.x, View.y, 1.f, 0.f);

		// ��viewMatrix���棬��Ϊ����Ҫ������ռ�ת�����ӿڿռ䣬����Ҫ����
		XMFLOAT4X4 viewFloat4 = camera->GetViewMatrix();
		XMMATRIX viewMatrix = XMLoadFloat4x4(&viewFloat4);
		XMVECTOR viewDet = XMMatrixDeterminant(viewMatrix);
		XMMATRIX InverseViewMatrix = XMMatrixInverse(&viewDet, viewMatrix);

		// ��ײ���
		FCollisionSceneQuery::RayCastSingleQuery(world, OriginPoint, Direction, InverseViewMatrix, OutHitResult);


	}

	return false;
}
