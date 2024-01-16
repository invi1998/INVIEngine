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

	ESelectedAxis GetSelectedAxis() const;

protected:
	CVARIABLE()
	CCustomMeshComponent* XAxisComponent = nullptr;	// X轴

	CVARIABLE()
	CCustomMeshComponent* YAxisComponent = nullptr;	// Y轴

	CVARIABLE()
	CCustomMeshComponent* ZAxisComponent = nullptr;	// Z轴

	CVARIABLE()
	CInputComponent* InputComponent = nullptr;	// 输入组件

	virtual void OnMouseMoved(int x, int y);

	virtual void OnMouseLeftDown(int x, int y);

	virtual void OnMouseLeftUp(int x, int y);

private:
	ESelectedAxis SelectedAxis = AXIS_NONE;	// 选中的轴向
};

