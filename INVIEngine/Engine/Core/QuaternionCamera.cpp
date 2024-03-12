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

extern CMeshComponent *SelectedAxisComponent; // ��ѡ�е�����
extern GActorObject* SelectedActor; // ��ѡ�е�����
extern GMoveArrow* MoveArrow;

GQuaternionCamera::GQuaternionCamera()
	: GClientViewPort()
{
	FCreateObjectParams params{};
	params.Owner = this;

	InputComponent = CreateObject<CInputComponent>(params, new CInputComponent());
	InputComponent->OnMouseWheelDelegate.Bind(this, &GQuaternionCamera::OnMouseScroll);
	// �󶨼�������¼�
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

	if (FInput::IsKeyPressed(VK_LMENU)) // ��סalt��
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
			// ������
			MouseRotate(delta);
		}
		else if (FInput::IsMouseButtonPressed(VK_MBUTTON))
		{
			// ����м�
			MousePan(delta);
		}
		else if (FInput::IsMouseButtonPressed(VK_RBUTTON))
		{
			// ����Ҽ�
			MouseZoom(delta.y);
		}
	}
	// ��סctrl��
	else if (FInput::IsKeyPressed(VK_LCONTROL) && CameraType==ECameraType::CameraRoaming) // ��ס��ctrl
	{
		const XMFLOAT2 &mouse{FInput::GetMouseX(), FInput::GetMouseY()};
		// ENGINE_LOG_WARNING("IsKeyPressed:(%f, %f)", mouse.x, mouse.y);
		XMFLOAT2 delta = {(mouse.x - m_InitialMousePosition.x) * 0.003f, (mouse.y - m_InitialMousePosition.y) * 0.003f};
		m_InitialMousePosition.x = mouse.x;
		m_InitialMousePosition.y = mouse.y;

		if (FInput::IsMouseButtonPressed(VK_LBUTTON))
		{
			// ������
			MouseStrafe(delta); // ��������ƽ�������
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
		// �л����������
		CameraType = CameraType == ECameraType::CameraRoaming ? ECameraType::ObservationObject : CameraRoaming;
	}
	else if (FInput::IsKeyReleased(VK_F1))
	{
		if (SelectedActor)
		{
			// �������������ѡ�е�����
			// FaceTarget(GetPosition(), SelectedActor->GetPosition());

			FTimelineDelegate TimelineDelegate{};
			TimelineDelegate.Bind(this, &GQuaternionCamera::LookAtAndMoveToSelectedObject);
			CameraTimeline.BindTimelineDelegate(TimelineDelegate, 1.f, false, false);
		}
	}
	else
	{
		// ��������������
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

	// �����������Ͱ���z����ת�������Ч��
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

	// �����������Ͱ���z����ת�������Ч��
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
			// renderLayerManage->Clear(static_cast<int>(EMeshRenderLayerType::RENDER_LAYER_SELECT));	// ���֮ǰ��ѡ��
			// renderLayerManage->Add(HitResult.HitRenderingData, static_cast<int>(EMeshRenderLayerType::RENDER_LAYER_SELECT));	// ���ѡ��
			SelectedActor = HitResult.HitActor;
			renderLayerManage->HighlightObject(HitResult.HitRenderingData);

			/*if (MoveArrow)
			{
				MoveArrow->SetPosition(HitResult.HitActor->GetPosition());
				MoveArrow->SetVisible(true);
			}*/
		}

		FOperationHandleSelectManage::Get()->SetNewSelectedObject(HitResult.HitActor); // ����ѡ�еĲ������
		FOperationHandleSelectManage::Get()->DisplaySelectedHandle();				   // ��ʾ
	}
	else
	{
		if (FRenderLayerManage *renderLayerManage = GetRenderLayerManager())
		{
			renderLayerManage->Clear(static_cast<int>(EMeshRenderLayerType::RENDER_LAYER_SELECT)); // ���֮ǰ��ѡ��
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

		// ��ȡѡ�������AABB����
		BoundingBox boundingBox = SelectedActor->GetBoundingBox();
		// ��ȡ���ӵİ뾶��Ȼ�����������İ뾶
		XMFLOAT3 extents = boundingBox.Extents;
		XMVECTOR boundingBoxSize = XMLoadFloat3(&extents);	// תΪ����
		float R = XMVectorGetX(XMVector3Length(boundingBoxSize));	// ���ӵİ뾶
		float H = 20.f;		// �������׵�ĸ߶ȵ��ں��ӵİ뾶����H������Hȡֵ��һ������ֵ�����Ը���ʵ���������
		float L = (R + H) / tan(FOV);	// �������Ŀ��ľ���

		// ��ȡ�������Forward������ͬʱ��λ���������������Forward�������������λ�õ�ѡ�������λ�õ����������������Ǹ��ģ� ����ֱ���������λ�ü�ȥ�������λ�ã�
		XMVECTOR CameraForward = XMVector3Normalize(ActorPos - CameraPos);

		XMVECTOR CameraEnd = ActorPos - L * CameraForward;	// �����������λ��

		XMVECTOR NewCameraPos = XMVectorLerp(CameraPos, CameraEnd, currentTime / duration);		// ��ֵ�����������λ��
		XMStoreFloat3(&CameraPosition, NewCameraPos);
		SetPosition(CameraPosition);

		// ������Ԫ����ֵ�㷨��������ӵ�ǰ�����ֵ��Ŀ�곯����Ԫ����ֵ��ת��
		// ��ʹ��Forward������Up������Right����������ת����Ȼ����ת����תΪ��Ԫ����Ȼ�������ٵõ�Ŀ�����ת����Ȼ��Ŀ�����ת����תΪ��Ԫ����Ȼ��������ʹ����Ԫ����ֵ�㷨�����������ת��ֵ��Ŀ�����ת
		// ��Ԫ����ֵ�㷨�кܶ��֣���һ�������Բ�ֵ���ڶ����������ֵ����������SLERP��ֵ����������NLERP��ֵ���������ǲ���SLERP��ֵ���������Բ�ֵ��
		// ͬʱ���ǻ���Ҫע�������Ԫ���Ĳ�ֵ���з���ģ�����������Ҫ�ж�һ��������ĳ����Ŀ�곯��ļнǣ�����нǴ���180�ȣ�������Ҫ��Ŀ�곯��ȡ����������Ԫ���Ĳ�ֵ�������˫���������⣩

		// CameraForward = -CameraForward;	// ȡ��
		XMFLOAT3 TargetForward{};
		XMStoreFloat3(&TargetForward, CameraForward);
		fvector_3d forwardFV = EngineMath::ToVector3d(TargetForward);

		if (IsQuatAnimationMode())
		{
			//// ��ȡ��ǰ�����������ת��Ԫ��
			//XMVECTOR CameraQuat = GetRotationQuat();
			//// ��һ����Ԫ��
			//CameraQuat = XMQuaternionNormalize(CameraQuat);
			//// ��ȡĿ�����ת��Ԫ��
			//XMVECTOR TargetQuat = EngineMath::BuildQuaternion(TargetForward);
			//TargetQuat = XMQuaternionNormalize(TargetQuat);
			//// ������Ԫ��SLERP��ֵ�㷨��ֵ���������ת
			//XMVECTOR NewCameraQuat = XMQuaternionSlerp(CameraQuat, TargetQuat, (currentTime / duration)*4);
			//SetRoationQuat(NewCameraQuat);

			// ��ȡ��ǰ�����������ת��Ԫ��
			fquat Q1 = GetRotationFQuat();
			fquat Q2 = EngineMath::BuildQuaternionFQuat(forwardFV);
			fquat Q = fquat::lerp(Q1, Q2, (currentTime / duration)*4);
			SetRoationFQuat(Q);
		}
		else  // ŷ���ǲ�ֵ
		{
			// ��ȡ��ǰ�����������תŷ����
			frotator CameraRotation = GetRotationFrotator();
			// ��ȡĿ�����תŷ����
			frotator TargetRotation = EngineMath::BuildRotationMatrix(forwardFV);
			// ����ŷ���ǲ�ֵ�㷨��ֵ���������ת
			frotator NewCameraRotation = EngineMath::Lerp(CameraRotation, TargetRotation, (currentTime / duration)*4.f);
			SetRotation(NewCameraRotation);
		}
		
	}
}
