#include "EngineMinimal.h"
#include "EngineVarableTable.h"

int ActorSelected = 0;
class GActorObject* SelectedActor = nullptr;	// 被选中的物体
class CMeshComponent* SelectedAxisComponent = nullptr;	// 被选中的轴向
class GMoveArrow* MoveArrow = nullptr;	// 平移箭头
class GScalingArrow* ScaleArrow = nullptr;	// 缩放箭头
class GRotateArrow* RotateArrow = nullptr;	// 旋转箭头
