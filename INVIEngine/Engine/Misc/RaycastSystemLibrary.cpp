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

		// 射线坐标原点（起点）视口下的坐标，表示点，所以其次为1
		XMVECTOR OriginPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		// 射线方向，z值为1，其次为0，因为是一个向量，所以其次为0，然后我们在计算公式的时候，假定的z值为1，所以这里z是1
		XMVECTOR Direction = XMVectorSet(View.x, View.y, 1.f, 0.f);

		// 对viewMatrix求逆，因为我们要从世界空间转换到视口空间，所以要求逆
		XMFLOAT4X4 viewFloat4 = camera->GetViewMatrix();
		XMMATRIX viewMatrix = XMLoadFloat4x4(&viewFloat4);
		XMVECTOR viewDet = XMMatrixDeterminant(viewMatrix);
		XMMATRIX InverseViewMatrix = XMMatrixInverse(&viewDet, viewMatrix);

		// 碰撞检测
		FCollisionSceneQuery::RayCastSingleQuery(world, OriginPoint, Direction, InverseViewMatrix, OutHitResult);


	}

	return false;
}
