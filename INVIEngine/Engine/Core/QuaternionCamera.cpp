#include "EngineMinimal.h"

#include "QuaternionCamera.h"

#include "OperationHandleSelectManage.h"
#include "Component/InputComponent.h"
#include "Component/TransformationComponent.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Config/EngineRenderConfig.h"
#include "Misc/RaycastSystemLibrary.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "SelectEditor/OperationHandle/MoveArrow.h"

FCaptureOnMousesWheelsDelegate MousesWheelsDelegate;
// FCaptureOnMousesMoveDelegate MouseMoveDelegate;

// const XMVECTOR GQuaternionCamera::DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
// const XMVECTOR GQuaternionCamera::DefaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
// const XMVECTOR GQuaternionCamera::DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

extern CMeshComponent *SelectedAxisComponent; // 被选中的轴向
extern GActorObject* SelectedActor; // 被选中的物体
extern GMoveArrow* MoveArrow;

GQuaternionCamera::GQuaternionCamera()
	: GClientViewPort()
{
	FCreateObjectParams params{};
	params.Owner = this;

	InputComponent = CreateObject<CInputComponent>(params, new CInputComponent());
	InputComponent->OnMouseWheelDelegate.Bind(this, &GQuaternionCamera::OnMouseScroll);
	// 绑定键盘鼠标事件
	InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &GQuaternionCamera::ExecuteInput);
	CameraType = ECameraType::CameraRoaming;

	// UpdateViewMatrix();
	GActorObject::SetPosition(XMFLOAT3{0.f, 0.f, 10.f});
	GActorObject::SetRotation(XMFLOAT3{0.f, 180.f, 0.f});
	UpdateProjectionMatrix();
	BuildMatrixByType();
}

void GQuaternionCamera::BeginInit()
{
	CCoreMinimalObject::BeginInit();
}

void GQuaternionCamera::Tick(float DeltaTime)
{
	GClientViewPort::Tick(DeltaTime);

	CameraTimeline.Tick(DeltaTime);
}

void GQuaternionCamera::ExecuteInput()
{
	OnUpdate(1.0f);
}

void GQuaternionCamera::MouseStrafe(const XMFLOAT2 &delta)
{
	XMFLOAT3 cameraPosition = GetPosition();
	XMVECTOR right = XMLoadFloat3(&GetRightVector());
	XMVECTOR position = XMLoadFloat3(&cameraPosition);

	XMVECTOR AmountMovement = XMVectorReplicate(delta.x * 10.f);
	XMStoreFloat3(&cameraPosition, XMVectorMultiplyAdd(AmountMovement, right, position));

	XMVECTOR up = XMLoadFloat3(&GetUpVector());
	XMVECTOR AmountMovementUp = XMVectorReplicate(delta.y * 10.f);
	XMStoreFloat3(&cameraPosition, XMVectorMultiplyAdd(AmountMovementUp, up, XMLoadFloat3(&cameraPosition)));

	SetPosition(cameraPosition);
}

void GQuaternionCamera::OnUpdate(float ts)
{

	if (FInput::IsKeyPressed(VK_LMENU)) // 按住alt键
	{
		const XMFLOAT2 &mouse{FInput::GetMouseX(), FInput::GetMouseY()};
		// ENGINE_LOG_WARNING("IsKeyPressed:(%f, %f)", mouse.x, mouse.y);
		XMFLOAT2 delta = {(mouse.x - m_InitialMousePosition.x) * 0.003f, (mouse.y - m_InitialMousePosition.y) * 0.003f};
		m_InitialMousePosition.x = mouse.x;
		m_InitialMousePosition.y = mouse.y;

		if (FInput::IsMouseButtonPressed(VK_LBUTTON))
		{
			if (CameraType == ECameraType::CameraRoaming)
			{
				float XRadians = XMConvertToRadians(delta.x * 10.f);
				float YRadians = XMConvertToRadians(delta.y * 10.f);

				RotateAroundXAxis(YRadians);
				RotateAroundYAxis(XRadians);
			}
			// 鼠标左键
			MouseRotate(delta);
		}
		else if (FInput::IsMouseButtonPressed(VK_MBUTTON))
		{
			// 鼠标中键
			MousePan(delta);
		}
		else if (FInput::IsMouseButtonPressed(VK_RBUTTON))
		{
			// 鼠标右键
			MouseZoom(delta.y);
		}
	}
	// 按住ctrl键
	else if (FInput::IsKeyPressed(VK_LCONTROL) && CameraType==ECameraType::CameraRoaming) // 按住左ctrl
	{
		const XMFLOAT2 &mouse{FInput::GetMouseX(), FInput::GetMouseY()};
		// ENGINE_LOG_WARNING("IsKeyPressed:(%f, %f)", mouse.x, mouse.y);
		XMFLOAT2 delta = {(mouse.x - m_InitialMousePosition.x) * 0.003f, (mouse.y - m_InitialMousePosition.y) * 0.003f};
		m_InitialMousePosition.x = mouse.x;
		m_InitialMousePosition.y = mouse.y;

		if (FInput::IsMouseButtonPressed(VK_LBUTTON))
		{
			// 鼠标左键
			MouseStrafe(delta); // 上下左右平移摄像机
		}

		if (FInput::IsKeyReleased(Key::W))
		{
			MoveForward(5.f);
		}
		if (FInput::IsKeyReleased(Key::S))
		{
			MoveForward(-5.f);
		}
		if (FInput::IsKeyReleased(Key::A))
		{
			MoveRight(2.f);
		}
		if (FInput::IsKeyReleased(Key::D))
		{
			MoveRight(-2.f);
		}
		if (FInput::IsKeyReleased(Key::E))
		{
			XMFLOAT2 delta = {2.f, 0.f};
			MouseRotate(delta);
		}
		if (FInput::IsKeyReleased(Key::Q))
		{
			XMFLOAT2 delta = {-2.f, 0.f};
			MouseRotate(delta);
		}
	}
	else if (FInput::IsKeyReleased(VK_TAB))
	{
		// 切换摄像机类型
		CameraType = CameraType == ECameraType::CameraRoaming ? ECameraType::ObservationObject : CameraRoaming;
	}
	else if (FInput::IsKeyReleased(VK_F1))
	{
		if (SelectedActor)
		{
			// 先让摄像机看向选中的物体
			FaceTarget(GetPosition(), SelectedActor->GetPosition());

			FTimelineDelegate TimelineDelegate{};
			TimelineDelegate.Bind(this, &GQuaternionCamera::LookAtAndMoveToSelectedObject);
			CameraTimeline.BindTimelineDelegate(TimelineDelegate, 1.f, false, false);
		}
	}
	else
	{
		// 如果是鼠标左键点击
		if (FInput::IsMouseButtonClicked(VK_LBUTTON))
		{
			if (!SelectedAxisComponent)
			{
				const XMFLOAT2 &mouse{FInput::GetMouseX(), FInput::GetMouseY()};
				OnClickScene(mouse);
			}
		}
	}

	BuildMatrixByType();
}

void GQuaternionCamera::SetViewportSize(int width, int height)
{
	ViewportWidth = width;
	ViewportHeight = height;
	UpdateProjectionMatrix();
}

void GQuaternionCamera::OnMouseScroll(int X, int Y, float InDelta)
{
	float zoom = InDelta / WHEEL_DELTA;

	MouseZoom(zoom);
	// UpdateViewMatrix();
}

void GQuaternionCamera::MouseRotate(const XMFLOAT2 &delta)
{
	float XRadians = XMConvertToRadians(delta.x * 10.f);
	float YRadians = XMConvertToRadians(delta.y * 10.f);

	Theta += -XRadians;
	Phi += YRadians;

	BuildMatrixByType();
}

void GQuaternionCamera::MousePan(const XMFLOAT2 &delta)
{
	auto [xSpeed, ySpeed] = PanSpeed();
	/*FocalPoint += -GetRightDirection() * delta.x * xSpeed * Distance;
	FocalPoint += GetUpDirection() * delta.y * ySpeed * Distance;*/
}

void GQuaternionCamera::MouseZoom(float delta)
{
	if (CameraType == ObservationObject)
	{
		Radius += delta * ZoomSpeed();
	}
	else if (CameraType == CameraRoaming)
	{
		float Distance = delta * ZoomSpeed();
		MoveForward(Distance);
	}
}

void GQuaternionCamera::MoveForward(float delta)
{
	if (CameraType == ECameraType::CameraRoaming)
	{
		XMFLOAT3 cameraPosition = GetPosition();
		XMVECTOR forward = XMLoadFloat3(&GetForwardVector());
		XMVECTOR position = XMLoadFloat3(&cameraPosition);

		XMVECTOR AmountMovement = XMVectorReplicate(delta * 1.f);
		XMStoreFloat3(&cameraPosition, XMVectorMultiplyAdd(AmountMovement, forward, position));
		SetPosition(cameraPosition);
	}
}

void GQuaternionCamera::MoveRight(float delta)
{
	if (CameraType == ECameraType::CameraRoaming)
	{
		XMFLOAT3 cameraPosition = GetPosition();
		XMVECTOR right = XMLoadFloat3(&GetRightVector());
		XMVECTOR position = XMLoadFloat3(&cameraPosition);

		XMVECTOR AmountMovement = XMVectorReplicate(-delta * 1.f);
		XMStoreFloat3(&cameraPosition, XMVectorMultiplyAdd(AmountMovement, right, position));
		SetPosition(cameraPosition);
	}
}

std::pair<float, float> GQuaternionCamera::PanSpeed() const
{
	float x = min(ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
	float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

	float y = min(ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
	float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	return {xFactor, yFactor};
}

float GQuaternionCamera::RotationSpeed() const
{
	return 0.8f;
}

float GQuaternionCamera::ZoomSpeed() const
{
	float distance = 10 * 0.2f;
	distance = max(distance, 0.2f);
	float speed = distance * distance;
	speed = min(speed, 50.0f); // max speed = 100
	return speed;
}

void GQuaternionCamera::RotateAroundXAxis(float rotateDegrees)
{
	XMVECTOR right = XMLoadFloat3(&GetRightVector());
	XMVECTOR forward = XMLoadFloat3(&GetForwardVector());
	XMVECTOR up = XMLoadFloat3(&GetUpVector());

	XMMATRIX RotationY = XMMatrixRotationAxis(right, rotateDegrees);

	// 计算各个方向和按照z轴旋转后的最终效果
	XMVECTOR upVector = XMVector3TransformNormal(up, RotationY);
	SetUpVector(upVector);

	XMVECTOR forwardVector = XMVector3TransformNormal(forward, RotationY);
	SetForwardVector(forwardVector);
}

void GQuaternionCamera::RotateAroundYAxis(float rotateDegrees)
{

	XMVECTOR right = XMLoadFloat3(&GetRightVector());
	XMVECTOR forward = XMLoadFloat3(&GetForwardVector());
	XMVECTOR up = XMLoadFloat3(&GetUpVector());

	XMMATRIX RotationZ = XMMatrixRotationY(rotateDegrees);

	// 计算各个方向和按照z轴旋转后的最终效果
	XMVECTOR rightVector = XMVector3TransformNormal(right, RotationZ);
	SetUpVector(rightVector);

	/*XMVECTOR upVector = XMVector3TransformNormal(up, RotationZ);
	SetUpVector(upVector);*/

	XMVECTOR forwardVector = XMVector3TransformNormal(forward, RotationZ);
	SetForwardVector(forwardVector);
}

void GQuaternionCamera::OnClickScene(const XMFLOAT2 &mousePos)
{
	// ENGINE_LOG_SUCCESS("pos: (%f, %f)", mousePos.x, mousePos.y);

	EngineType::FHitResult HitResult{};
	FRayCastSystemLibrary::GetHitResultByScreen(GetWorld(), mousePos, HitResult);

	if (HitResult.bHit)
	{
		if (SelectedActor == HitResult.HitActor)
		{
			return;
		}

		ENGINE_LOG_SUCCESS("HitResult: (%f, %f, %f)", HitResult.HitPoint.x, HitResult.HitPoint.y, HitResult.HitPoint.z);
		ENGINE_LOG_SUCCESS("HitResult: (%f, %f, %f)", HitResult.HitNormal.x, HitResult.HitNormal.y, HitResult.HitNormal.z);
		ENGINE_LOG_SUCCESS("HitResult: (%f)", HitResult.HitDistance);

		if (FRenderLayerManage *renderLayerManage = GetRenderLayerManager())
		{
			// renderLayerManage->Clear(static_cast<int>(EMeshRenderLayerType::RENDER_LAYER_SELECT));	// 清空之前的选中
			// renderLayerManage->Add(HitResult.HitRenderingData, static_cast<int>(EMeshRenderLayerType::RENDER_LAYER_SELECT));	// 添加选中
			SelectedActor = HitResult.HitActor;
			renderLayerManage->HighlightObject(HitResult.HitRenderingData);

			/*if (MoveArrow)
			{
				MoveArrow->SetPosition(HitResult.HitActor->GetPosition());
				MoveArrow->SetVisible(true);
			}*/
		}

		FOperationHandleSelectManage::Get()->SetNewSelectedObject(HitResult.HitActor); // 设置选中的操作句柄
		FOperationHandleSelectManage::Get()->DisplaySelectedHandle();				   // 显示
	}
	else
	{
		if (FRenderLayerManage *renderLayerManage = GetRenderLayerManager())
		{
			renderLayerManage->Clear(static_cast<int>(EMeshRenderLayerType::RENDER_LAYER_SELECT)); // 清空之前的选中
		}
		SelectedActor = nullptr;
		FOperationHandleSelectManage::Get()->SetNewSelectedObject(nullptr);
		FOperationHandleSelectManage::Get()->AllOperationHandleHide();
		/*if (MoveArrow)
		{
			MoveArrow->SetVisible(false);
		}*/
	}
}

void GQuaternionCamera::BuildMatrixByType()
{
	switch (CameraType)
	{
	case CameraRoaming:
	{
		BuildViewMatrix();

		break;
	}
	case ObservationObject:
	{
		XMFLOAT3 CameraPosition = GetPosition();
		CameraPosition.x = Radius * sinf(Phi) * cosf(Theta);
		CameraPosition.z = Radius * sinf(Phi) * sinf(Theta);
		CameraPosition.y = Radius * cosf(Phi);

		XMVECTOR Pos = XMVectorSet(CameraPosition.x, CameraPosition.y, CameraPosition.z, 1.0f);
		XMVECTOR ViewTarget = XMVectorZero();
		XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

		XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);

		XMStoreFloat4x4(&ViewMatrix, ViewLookAt);

		break;
	}
	}
}

void GQuaternionCamera::LookAtAndMoveToSelectedObject(float currentTime, float duration)
{
	ENGINE_LOG_WARNING("LookAtAndMoveToSelectedObject: (%f, %f)", currentTime, duration);

	if (SelectedActor)
	{
		XMFLOAT3 CameraPosition = GetPosition();
		XMVECTOR CameraPos = XMLoadFloat3(&CameraPosition);
		XMVECTOR ActorPos = XMLoadFloat3(&SelectedActor->GetPosition());

		// 获取选中物体的AABB盒子
		BoundingBox boundingBox = SelectedActor->GetBoundingBox();
		// 获取盒子的半径，然后计算摄像机的半径
		XMFLOAT3 extents = boundingBox.Extents;
		XMVECTOR boundingBoxSize = XMLoadFloat3(&extents);	// 转为向量
		float R = XMVectorGetX(XMVector3Length(boundingBoxSize));	// 盒子的半径
		float H = 20.f;		// 摄像机视椎的高度等于盒子的半径加上H，这里H取值是一个经验值，可以根据实际情况调整
		float L = (R + H) / tan(FOV);	// 摄像机到目标的距离

		// 获取摄像机的Forward向量，同时单位化向量（这里这个Forward向量是摄像机的位置到选中物体的位置的向量，所以这里是负的， 我们直接用物体的位置减去摄像机的位置）
		XMVECTOR CameraForward = XMVector3Normalize(ActorPos - CameraPos);

		XMVECTOR CameraEnd = ActorPos - L * CameraForward;	// 摄像机的最终位置

		XMVECTOR NewCameraPos = XMVectorLerp(CameraPos, CameraEnd, currentTime / duration);
		XMStoreFloat3(&CameraPosition, NewCameraPos);
		SetPosition(CameraPosition);

		// ENGINE_LOG_WARNING("CameraPosition: (%f, %f, %f)", CameraPosition.x, CameraPosition.y, CameraPosition.z);

		// ENGINE_LOG_WARNING("ActorPos: (%f, %f, %f)", ActorPos.x, ActorPos.y, ActorPos.z);
	}
}
