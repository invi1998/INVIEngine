#pragma once
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

	void ExecuteInput();

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

	float GetMouseMoveDistance(int x, int y, XMVECTOR& ActorLocation, XMVECTOR& DragDirection);

private:
	ESelectedAxis SelectedAxis = AXIS_NONE;	// 选中的轴向

	float FixedZoom = 30.f;	// 固定的缩放比例

	// 是否按世界坐标系操作
	bool bWorldOperate = true;
};

