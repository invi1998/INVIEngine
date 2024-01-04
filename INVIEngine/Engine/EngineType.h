#pragma once

struct FRenderingData;
class GActorObject;
class CComponent;

namespace EngineType
{
	// 命中结果
	struct FHitResult
	{
		bool bHit = false;					// 是否命中
		XMFLOAT3 HitPoint{};				// 命中点
		XMFLOAT3 HitNormal{};				// 命中点法线
		XMFLOAT3 HitDirection{};			// 命中的方向

		float HitDistance=0;			// 命中距离
		float Time=0;				// 命中时间


		CComponent* HitComponent = nullptr;	// 命中的组件
		GActorObject* HitActor=nullptr;		// 命中的Actor

		std::weak_ptr<FRenderingData> HitRenderingData;	// 命中的渲染数据
		
	};
}


