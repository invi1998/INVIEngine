#include "EngineMinimal.h"
#include "EngineVarableTable.h"

int ActorSelected = 0;
class GActorObject* SelectedActor = nullptr;	// ��ѡ�е�����
class CMeshComponent* SelectedAxisComponent = nullptr;	// ��ѡ�е�����
class GMoveArrow* MoveArrow = nullptr;	// ƽ�Ƽ�ͷ
class GScalingArrow* ScaleArrow = nullptr;	// ���ż�ͷ
class GRotateArrow* RotateArrow = nullptr;	// ��ת��ͷ
