#pragma once
#include "OperationHandleSelectManage.h"
#include "Actor/Core/ActorObject.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Interface/DirectXDeviceInterface.h"

class CInputComponent;
class CCustomMeshComponent;

// 选中的轴向
enum ESelectedAxis
{
	AXIS_NONE = 0,
	AXIS_X = 1,		// 对应物体的Right方向
	AXIS_Y = 2,		// 对应物体的Up方向
	AXIS_Z = 3,		// 对应物体的Forward方向
	AXIS_ANY = 4,	// 任意方向
};

class GOperationHandle : public GActorObject, public IDirectXDeviceInterface
{
public:
	GOperationHandle();
	virtual ~GOperationHandle() override = default;

	virtual void SetMesh() = 0;

	virtual void SetMeshRenderLayerType(EMeshRenderLayerType mesh_render_layer);

	void ResetColor();

	void ResetColor(CCustomMeshComponent* axis_component, const XMFLOAT4& color);

	void BeginInit() override;

	virtual void ExecuteInput() = 0;		// 该接口主要用来处理键盘鼠标事件，要求子类必须进行实现

	void Tick(float DeltaTime) override;

	void SetPosition(const XMVECTOR& InNewPosition) override;

	void SetPosition(const XMFLOAT3& InNewPosition) override;

	void SetVisible(bool visible);

	void SetVisible(bool visible, CCustomMeshComponent* axis_component);

	void SetScale(const XMFLOAT3& InNewScale) override;

	bool IsVisible();

	ESelectedAxis GetSelectedAxis() const;

	void SetWorldOperate(bool bWorld) { bWorldOperate = bWorld; }

	bool IsWorldOperate() const { return bWorldOperate; }

protected:
	CVARIABLE()
	CCustomMeshComponent* XAxisComponent = nullptr;	// X轴

	CVARIABLE()
	CCustomMeshComponent* YAxisComponent = nullptr;	// Y轴

	CVARIABLE()
	CCustomMeshComponent* ZAxisComponent = nullptr;	// Z轴

	CVARIABLE()
	CCustomMeshComponent* AnyAxisComponent = nullptr;	// 任意轴

	CVARIABLE()
	CInputComponent* InputComponent = nullptr;	// 输入组件

	virtual void OnMouseMoved(int x, int y);

	virtual void OnMouseLeftDown(int x, int y);

	virtual void OnMouseLeftUp(int x, int y);

	// 需要子类实现的获取任意轴反向的虚函数，传入世界坐标系的原点和方向和实体位置，返回任意轴的方向
	virtual void GetAnyAxisDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation, XMVECTOR& OutDirection) = 0;

	float GetMouseMoveDistance(int x, int y, XMVECTOR& ActorLocation, XMVECTOR& DragDirection);

	bool IsCurrentSelectedHandle();

	// 判断选择的操作句柄是否当前特定子类型
	template<typename T>
	bool IsCurrentSelectedHandleType()
	{
		return dynamic_cast<T*>(this) != FOperationHandleSelectManage::Get()->GetSelectedOperationHandle();
	}

private:
	ESelectedAxis SelectedAxis = AXIS_NONE;	// 选中的轴向

	float FixedZoom = 30.f;	// 固定的缩放比例

	// 是否按世界坐标系操作
	bool bWorldOperate = true;
};

