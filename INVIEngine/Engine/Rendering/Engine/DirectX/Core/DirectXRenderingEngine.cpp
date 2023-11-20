#include "EngineMinimal.h"

#include "DirectXRenderingEngine.h"

#include "Actor/Light/ParallelLight.h"
#include "Actor/Light/PointLight.h"
#include "Actor/Light/SpotLight.h"
#include "Actor/Sky/Fog.h"
#include "Config/EngineRenderConfig.h"
#include "Core/World.h"
#include "Manage/LightManager.h"
#include "Material/Core/Material.h"
#include "Material/Core/MaterialType.h"
#include "Mesh/BoxMesh.h"
#include "Mesh/ConeMesh.h"
#include "Mesh/CustomMesh.h"
#include "Mesh/CylinderMesh.h"
#include "Mesh/PipeMesh.h"
#include "Mesh/PlaneMesh.h"
#include "Mesh/PyramidMesh.h"
#include "Mesh/SphereMesh.h"
#include "Mesh/TorusMesh.h"

class GBoxMesh;

CDirectXRenderingEngine::CDirectXRenderingEngine()
	: CurrentFenceIndex(0),
	CurrentSwapBufferIndex(0),
	M4XNumQualityLevels(0),
	bMSAA4XEnabled(false),
	BackBufferFormat(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM), // �����ʽ Ĭ������Ϊ 8λ�޷��Ź�һ��RGBA��ʽ����0-255��rgbaֵ ӳ�䵽 0-1��
	DepthStencilFormat(DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT)
{

	for (UINT i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.push_back(ComPtr<ID3D12Resource>());
	}

	bTick = false;

	MeshManage = CreateObject<CMeshManager>(new CMeshManager);
	LightManager = CreateObject<CLightManager>(new CLightManager());

}

CDirectXRenderingEngine::~CDirectXRenderingEngine()
{
	delete MeshManage;
	delete LightManager;
	delete World;
}

int CDirectXRenderingEngine::PreInit(FWinMainCommandParameters InParameters)
{
	

	return 0;
}

int CDirectXRenderingEngine::Init(FWinMainCommandParameters InParameters)
{

	InitDirect3D();

	PostInitDirect3D();

	MeshManage->Init();

	return 0;
}

int CDirectXRenderingEngine::PostInit()
{
	// ��ʼ�������б�
	ANALYSIS_RESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), nullptr));

	//{

	//	// ƽ�й�
	//	//if (GParallelLight* ParallelLight = World->CreateActorObject<GParallelLight>())
	//	//{
	//	//	ParallelLight->SetPosition(XMFLOAT3(0.f, 10.f, -20.f));
	//	//	ParallelLight->SetRotation(fvector_3d(0.f, 0.f, 0.f));
	//	//	ParallelLight->SetLightIntensity({ 1.0f, 1.0f, 1.1f });

	//	//	/*if (CMaterial* InMaterial = (*ParallelLight->GetMaterial())[0])
	//	//	{
	//	//		InMaterial->SetBaseColor(XMFLOAT4{ Colors::OrangeRed });
	//	//		InMaterial->SetMaterialType(EMaterialType::BaseColor);
	//	//		InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);

	//	//		InMaterial->SetRoughness(1.0f);
	//	//	}*/
	//	//}

	//	// ���
	//	if (GPointLight* PointLight = World->CreateActorObject<GPointLight>())
	//	{
	//		PointLight->SetPosition(XMFLOAT3(0.f, 2.f, 10.f));
	//		PointLight->SetRotation(fvector_3d(0.f));
	//		PointLight->SetLightIntensity(XMFLOAT3{ 0.9f, 0.9f, 0.9f });

	//		PointLight->SetStartAttenuation(0.0f);
	//		PointLight->SetEndAttenuation(600.f);

	//		PointLight->SetKc(1.f);
	//		PointLight->SetKl(0.007f);
	//		PointLight->SetKq(0.0002f);
	//	}

	//	//// �۹�
	//	//if (GSpotLight* SpotLight = World->CreateActorObject<GSpotLight>())
	//	//{
	//	//	SpotLight->SetPosition(XMFLOAT3(20.f, 10.f, -20.f));
	//	//	SpotLight->SetLightIntensity(XMFLOAT3{ 2.f, 2.f, 1.43f });

	//	//	SpotLight->SetStartAttenuation(0.0f);
	//	//	SpotLight->SetEndAttenuation(200.0f);

	//	//	SpotLight->SetKc(1.f);
	//	//	SpotLight->SetKl(0.022f);
	//	//	SpotLight->SetKq(0.0019f);

	//	//	SpotLight->SetConicalInnerCorner(15.f);
	//	//	SpotLight->SetConicalOuterCorner(20.f);

	//	//}

	//	
	//	if (GSphereMesh* sphere = World->CreateActorObject<GSphereMesh>())
	//	{
	//		sphere->SetMeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_BACKGROUND);

	//		sphere->CreateMesh(2.f, 100, 100, true);
	//		sphere->SetPosition({ 0.f, 0.f, 0.f });
	//		sphere->SetScale(fvector_3d(4000.f));

	//		if (CMaterial* material = (*sphere->GetMaterial())[0])
	//		{
	//			material->SetBaseColorIndexKey("x1_CubeMap");
	//			material->SetSpecular(XMFLOAT3(1.0f, 1.0f, 1.0f));
	//			material->SetMaterialType(BaseColor);
	//		}
	//	}

	//	if (GPlaneMesh* PlaneMesh = World->CreateActorObject<GPlaneMesh>())
	//	{
	//		PlaneMesh->CreateMesh(400.f, 400.f, 2, 2);
	//		PlaneMesh->SetPosition(XMFLOAT3(0.f, -2.f, 0.f));
	//		// PlaneMesh->SetScale(fvector_3d(40.f, 40.f, 40.f));

	//		if (CMaterial* InMaterial = (*PlaneMesh->GetMaterial())[0])
	//		{
	//			// InMaterial->SetBaseColorIndexKey("Earth");
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::Gray));
	//			InMaterial->SetMaterialType(EMaterialType::Lambert);

	//			InMaterial->SetRoughness(1.0f);
	//		}
	//	}

	//	////����Ȧ
	//	if (GTorusMesh* InTorusMesh = World->CreateActorObject<GTorusMesh>())
	//	{
	//		InTorusMesh->CreateMesh(6.f, 2.f, 40.f, 40.f);
	//		InTorusMesh->SetPosition(XMFLOAT3(-6.f, -2.f, 30.f));
	//		InTorusMesh->SetScale(fvector_3d(1.f));
	//		if (CMaterial* InMaterial = (*InTorusMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetMaterialType(EMaterialType::HalfLambert);
	//		}
	//	}


	//	//����׶
	//	if (GPyramidMesh* InPyramidMesh = World->CreateActorObject<GPyramidMesh>())
	//	{
	//		InPyramidMesh->CreateMesh(EPyramidNumberSides::Pyramid_3, 1);
	//		InPyramidMesh->SetPosition(XMFLOAT3(-1.f, 2.f, 30.f));
	//		InPyramidMesh->SetRotation(fvector_3d(0.f, 90.f, 0.f));
	//		InPyramidMesh->SetScale(fvector_3d(1.f));
	//		if (CMaterial* InMaterial = (*InPyramidMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor({ 4.f, 0.f, 0.f, 1.f });
	//			InMaterial->SetMaterialType(EMaterialType::HalfLambert);
	//		}
	//	}

	//	//Pipeģ��
	//	if (GPipeMesh* InPipeMesh = World->CreateActorObject<GPipeMesh>())
	//	{
	//		InPipeMesh->CreateMesh(3.f, 3.f, 6.f, 1.f, 20.f, 20.f);
	//		InPipeMesh->SetPosition(XMFLOAT3(-9.f, 2.f, 30.f));
	//		if (CMaterial* InMaterial = (*InPipeMesh->GetMaterial())[0])
	//		{
	//			//InMaterial->SetBaseColor(fvector_4d(5.f));
	//			//InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
	//			InMaterial->SetMaterialType(EMaterialType::HalfLambert);
	//		}
	//	}

	//	//׶��
	//	if (GConeMesh* InConeMesh = World->CreateActorObject<GConeMesh>())
	//	{
	//		InConeMesh->CreateMesh(2.f, 3.f, 20.f, 20.f);

	//		InConeMesh->SetPosition(XMFLOAT3(7.f, -2.f, 30.f));
	//		InConeMesh->SetScale(fvector_3d(1.f, 2.f, 1.f));
	//		if (CMaterial* InMaterial = (*InConeMesh->GetMaterial())[0])
	//		{
	//			//	InMaterial->SetBaseColor(fvector_4d(1.f));
	//			InMaterial->SetMaterialType(EMaterialType::HalfLambert);
	//		}
	//	}

	//	//������
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 20, 20);

	//		SphereMesh->SetPosition(XMFLOAT3(-9.f, 2, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColorIndexKey("Earth");
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::White));
	//			InMaterial->SetMaterialType(EMaterialType::Lambert);

	//			InMaterial->SetRoughness(1.0f);
	//		}
	//	}

	//	//��������
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(-3.f, 2, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColorIndexKey("Wood");
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::White));
	//			InMaterial->SetMaterialType(EMaterialType::HalfLambert);

	//			InMaterial->SetRoughness(1.0f);
	//		}
	//	}

	//	//Phong
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(3.f, 2, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::White));
	//			InMaterial->SetMaterialType(EMaterialType::Phong);

	//			InMaterial->SetRoughness(0.8f);
	//		}
	//	}

	//	//BlinnPhong
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(9.f, 2, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::White));
	//			InMaterial->SetMaterialType(EMaterialType::Phong);

	//			InMaterial->SetRoughness(0.1f);
	//		}
	//	}

	//	//Fresnel
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(-9.f, 8, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::White));
	//			InMaterial->SetMaterialType(EMaterialType::Fresnel);

	//			// InMaterial->SetRoughness(1.0f);
	//		}
	//	}

	//	// WrapLight
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(-3.f, 8, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor({
	//				221.f / 255.f,
	//				154.f / 255.f,
	//				139.f / 255.f,1.f });
	//			InMaterial->SetMaterialType(EMaterialType::WrapLight);

	//			// InMaterial->SetRoughness(0.1f);
	//		}
	//	}

	//	// Minnaert
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(3.f, 8, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor({
	//				0.9f,
	//				0.9f,
	//				1.0f, 1.f });
	//			InMaterial->SetMaterialType(EMaterialType::Minnaert);

	//			InMaterial->SetRoughness(0.95f);
	//		}
	//	}

	//	// Banded
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(9.f, 8, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor({ XMFLOAT4(Colors::White) });
	//			InMaterial->SetMaterialType(EMaterialType::Banded);
	//		}
	//	}

	//	// GradualBanded
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(-9.f, 14, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor({
	//				3.f/255.f,169.f/255.f,244.f/255.f, 1.f });
	//			InMaterial->SetMaterialType(EMaterialType::GradualBanded);
	//		}
	//	}

	//	// CustomBanded
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(-3.f, 14, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor({
	//				255.f/255.f,87.f/255.f,34.f/255.f,1.f });
	//			InMaterial->SetMaterialType(EMaterialType::CustomBanded);
	//			InMaterial->SetRoughness(0.42f);
	//		}
	//	}

	//	// Back
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(3.f, 14, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor({
	//				2.f / 255.f,
	//				214.f / 255.f,
	//				17.f / 255.f, 1.f});
	//			InMaterial->SetMaterialType(EMaterialType::Back);
	//			InMaterial->SetRoughness(0.15f);
	//		}
	//	}

	//	// �������ԣ�ͷ����Ⱦ��AnisotoropyKaijiyakay
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(9.f, 14, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor({
	//				2.f / 255.f,
	//				214.f / 255.f,
	//				17.f / 255.f, 1.f });
	//			InMaterial->SetMaterialType(EMaterialType::AnisotoropyKaijiyakay);
	//			InMaterial->SetRoughness(0.15f);
	//		}
	//	}

	//	// OrenNayar �ֲڱ���
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(-9.f, 20, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor({
	//				121.f/255.f,85.f/255.f,72.f/255.f, 1.f });
	//			InMaterial->SetMaterialType(EMaterialType::OrenNayar);
	//			InMaterial->SetRoughness(3.15f);
	//		}
	//	}
	//	// OrenNayar �ֲڱ���
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(-3.f, 20, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::White));
	//			InMaterial->SetMaterialType(EMaterialType::PBR);
	//			InMaterial->SetRoughness(3.15f);
	//		}
	//	}

	//	// ������ʾ������ɫ
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(-3.f, 26, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::White));
	//			InMaterial->SetMaterialType(EMaterialType::BaseColor);
	//			InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
	//		}
	//	}

	//	// ������ʾ������ɫ
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(3.f, 26, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::White));
	//			InMaterial->SetMaterialType(EMaterialType::Normal);
	//		}
	//	}

	//	// ������ʾ������ɫ
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(9.f, 26, 0.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::White));
	//			InMaterial->SetMaterialType(EMaterialType::WorldNormal);
	//		}
	//	}

	//	// ƽ�й�ģ��
	//	if (GCustomMesh* SphereMesh = World->CreateActorObject<GCustomMesh>())
	//	{
	//		SphereMesh->CreateMesh("Asserts/Mesh/SunMesh.obj");

	//		SphereMesh->SetPosition(XMFLOAT3(19.f, 2, 19.f));
	//		SphereMesh->SetScale(fvector_3d(1.f, 1.f, 1.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::Bisque));
	//			InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
	//			InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::TriangleDisplay);
	//		}
	//	}

	//	// ��������
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(15.f, 2, 19.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColorIndexKey("MMOARPG");
	//			InMaterial->SetBaseColor(XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f));
	//			InMaterial->SetNormalIndexKey("MMOARPG_NRM");
	//			InMaterial->SetMaterialType(EMaterialType::OrenNayar);
	//		}
	//	}

	//	// ����ʯ����
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(9.f, 2, 19.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColorIndexKey("Earth");
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::Gray));
	//			InMaterial->SetSpecular(XMFLOAT3(Colors::White));
	//			InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
	//			InMaterial->SetRoughness(0.04f);
	//			InMaterial->SetFresnelF0({0.04f, 0.04f, 0.04f});
	//		}
	//	}

	//	// ľͷ����
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(3.f, 2, 19.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColorIndexKey("Wood");
	//			InMaterial->SetNormalIndexKey("Wood_NRM");
	//			InMaterial->SetBaseColor(XMFLOAT4(Colors::White));
	//			InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
	//		}
	//	}

	//	// ľͷ����2
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(-3.f, 2, 19.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			// InMaterial->SetBaseColorIndexKey("Wood2");
	//			InMaterial->SetNormalIndexKey("Wood2_Nor");
	//			InMaterial->SetSpecular("Wood2_SPEC");
	//			InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
	//		}
	//	}


	//	// ͸������
	//	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	//	{
	//		SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_TRANSPARENT);

	//		SphereMesh->CreateMesh(2.f, 50, 50);

	//		SphereMesh->SetPosition(XMFLOAT3(-9.f, 2, 19.f));
	//		if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//		{
	//			InMaterial->SetBaseColorIndexKey("TransparentContent");
	//			InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
	//		}
	//	}

	//	// ��
	//	if (const GFog* fog = World->CreateActorObject<GFog>())
	//	{
	//		fog->SetFogColor(XMFLOAT4{ Colors::AliceBlue });
	//		fog->SetFogStart(10.f);
	//		fog->SetFogRange(200.f);
	//		fog->SetFogHeight(10000.f);
	//		fog->SetFogTransparentCoefficient(0.01f);
	//	}


	//	//// ƽ�й�ģ��
	//	//if (GCustomMesh* SphereMesh = World->CreateActorObject<GCustomMesh>())
	//	//{
	//	//	SphereMesh->CreateMesh("Asserts/Mesh/SunMesh.obj");

	//	//	SphereMesh->SetPosition(XMFLOAT3(3.f, 12, 19.f));
	//	//	SphereMesh->SetScale(fvector_3d(1.f, 1.f, 1.f));
	//	//	if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//	//	{
	//	//		InMaterial->SetBaseColor(XMFLOAT4(Colors::Bisque));
	//	//		InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
	//	//		InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::TriangleDisplay);
	//	//	}
	//	//}

	//	//// �����ģ��
	//	//if (GCustomMesh* SphereMesh = World->CreateActorObject<GCustomMesh>())
	//	//{
	//	//	SphereMesh->CreateMesh("Asserts/Mesh/CameraMesh.obj");

	//	//	SphereMesh->SetPosition(XMFLOAT3(-19.f, 2, 19.f));
	//	//	SphereMesh->SetScale(fvector_3d(1.f, 1.f, 1.f));
	//	//	if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
	//	//	{
	//	//		InMaterial->SetBaseColor(XMFLOAT4(Colors::Bisque));
	//	//		InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
	//	//		InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::TriangleDisplay);
	//	//	}
	//	//}

	//}

	{
		//����Mesh
		/*if (GMesh* BoxMesh = MeshManage->CreateBoxMesh(4.f, 3.f, 1.5f))
		{
			BoxMesh->SetPosition(XMFLOAT3(4,3,5));
			BoxMesh->SetRotation(fvector_3d(60.f, 1.f, 20.f));
		}*/

		////�ƹ�����
		//if (GParallelLight* ParallelLight = World->CreateActorObject<GParallelLight>())
		//{
		//	ParallelLight->SetPosition(XMFLOAT3(10.f, -10.f, 10.f));
		//	ParallelLight->SetRotation(fvector_3d(0.f, 0.f, 0.f));
		//	ParallelLight->SetScale(fvector_3d(1));
		//	ParallelLight->SetLightIntensity(fvector_3d(1.1f,1.1f,1.1f));
		//}
		//��ƹ�����
		if (GPointLight* PointLight = World->CreateActorObject<GPointLight>())
		{
			PointLight->SetPosition(XMFLOAT3(0.f, -6.f, 10.f));
			PointLight->SetRotation(fvector_3d(0.f, 0.f, 0.f));

			PointLight->SetLightIntensity(XMFLOAT3(0.9f, 0.9f, 0.9f));
			PointLight->SetEndAttenuation(190.f);
		}

		//�۵ƹ�����
		//if (GSpotLight* SpotLight = World->CreateActorObject<GSpotLight>())
		//{
		//	SpotLight->SetPosition(XMFLOAT3(0.f, 10.f, -10.f));
		//	SpotLight->SetRotation(fvector_3d(0.f, 0.f, 0.f));
		//
		//	SpotLight->SetLightIntensity(fvector_3d(1.f));
		//	//SpotLight->SetStartAttenuation(1.f);
		//	SpotLight->SetEndAttenuation(130.f);
		//
		//	SpotLight->SetConicalInnerCorner(40.f);
		//	SpotLight->SetConicalOuterCorner(60.f);
		//}

		if (GSphereMesh* sphere = World->CreateActorObject<GSphereMesh>())
		{
			sphere->SetMeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_BACKGROUND);

			sphere->CreateMesh(2.f, 100, 100, true);
			sphere->SetPosition({ 0.f, 0.f, 0.f });
			sphere->SetScale(fvector_3d(4000.f));

			if (CMaterial* material = (*sphere->GetMaterial())[0])
			{
				material->SetBaseColorIndexKey("x1_CubeMap");
				material->SetSpecular(XMFLOAT3(1.0f, 1.0f, 1.0f));
				material->SetMaterialType(BaseColor);
			}
		}

		////����Ȧ
		if (GTorusMesh* InTorusMesh = World->CreateActorObject<GTorusMesh>())
		{
			InTorusMesh->CreateMesh(6.f, 2.f, 40.f, 40.f);
			InTorusMesh->SetPosition(XMFLOAT3(-22.f, -8, 20.f));
			InTorusMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InTorusMesh->GetMaterial())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);

				InMaterial->SetBaseColorIndexKey("Zhuan");
				InMaterial->SetNormalIndexKey("Zhuan_NRM");
			}
		}

		//����׶
		if (GPyramidMesh* InPyramidMesh = World->CreateActorObject<GPyramidMesh>())
		{
			InPyramidMesh->CreateMesh(EPyramidNumberSides::Pyramid_3, 1);
			InPyramidMesh->SetPosition(XMFLOAT3(-1.f, -8, 20.f));
			InPyramidMesh->SetRotation(fvector_3d(0.f, 90.f, 0.f));
			InPyramidMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InPyramidMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(4.f, 0.f, 0.f, 1.f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		//Pipeģ��
		if (GPipeMesh* InPipeMesh = World->CreateActorObject<GPipeMesh>())
		{
			InPipeMesh->CreateMesh(3.f, 3.f, 6.f, 1.f, 20.f, 20.f);
			InPipeMesh->SetPosition(XMFLOAT3(-9.f, -9, 20.f));
			InPipeMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InPipeMesh->GetMaterial())[0])
			{
				//InMaterial->SetBaseColor(fvector_4d(5.f));
				//InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);

				InMaterial->SetBaseColorIndexKey("Zhuan");
				InMaterial->SetNormalIndexKey("Zhuan_NRM");
			}
		}

		//׶��
		if (GConeMesh* InConeMesh = World->CreateActorObject<GConeMesh>())
		{
			InConeMesh->CreateMesh(2.f, 3.f, 20.f, 20.f);

			InConeMesh->SetPosition(XMFLOAT3(7.f, -11.f, 20.f));
			InConeMesh->SetScale(fvector_3d(1.f, 1.f, 1.f));
			if (CMaterial* InMaterial = (*InConeMesh->GetMaterial())[0])
			{
				//	InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		{
			InBoxMesh->CreateMesh(5.f, 5.f, 5.f);

			InBoxMesh->SetPosition(XMFLOAT3(22.f, -10.f, 20.f));
			InBoxMesh->SetScale(fvector_3d(1));
			if (CMaterial* InMaterial = (*InBoxMesh->GetMaterial())[0])
			{
				//	InMaterial->SetBaseColor(fvector_4d(0.5f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GCylinderMesh* InCylinderMesh = World->CreateActorObject<GCylinderMesh>())
		{
			InCylinderMesh->CreateMesh(2.f, 2.f, 5.f, 20.f, 20.f);

			InCylinderMesh->SetPosition(XMFLOAT3(14.f, -10.f, 20.f));
			InCylinderMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InCylinderMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColorIndexKey("Zhuan");
				InMaterial->SetNormalIndexKey("Zhuan_NRM");

				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GPlaneMesh* InPlaneMesh = World->CreateActorObject<GPlaneMesh>())
		{
			InPlaneMesh->CreateMesh(4.f, 3.f, 20, 20);

			InPlaneMesh->SetPosition(XMFLOAT3(0.f, -12.f, 0.f));
			InPlaneMesh->SetScale(fvector_3d(50.f, 1.f, 50.f));
			if (CMaterial* InMaterial = (*InPlaneMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}
		}

		//������
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 2, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}
		}

		//��������
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 2, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(
					221.f / 255.f,
					154.f / 255.f,
					255.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		//phong
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 2, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				//ģ���ͭ
				InMaterial->SetBaseColor(XMFLOAT4(
					191.f / 255.f,
					173.f / 255.f,
					111.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::Phong);
				InMaterial->SetSpecular(XMFLOAT3(1.f, 1.f, 1.f));

				InMaterial->SetFresnelF0(XMFLOAT3(0.08f, 0.08f, 0.08f));
				InMaterial->SetRoughness(0.4f);
			}
		}

		//blinn-phong
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(
					220.f / 255.f,
					223.f / 255.f,
					227.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
				InMaterial->SetSpecular(XMFLOAT3(1.f, 1.f, 1.f));
				InMaterial->SetRoughness(0.3f);

				InMaterial->SetFresnelF0(XMFLOAT3(0.1f, 0.1f, 0.1f));
			}
		}

		//�����
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::Fresnel);

				InMaterial->SetRoughness(0.8f);
			}
		}

		//Wrap ģ��Ƥ��
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(
					234.f / 255.f,
					154.f / 255.f,
					139.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::WrapLight);
			}
		}

		//Minnaert
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(
					0.9f,
					0.9f,
					1.0f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::Minnaert);

				InMaterial->SetRoughness(0.95f);
			}
		}

		//AnisotropyKajiyaKay
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 2, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColorIndexKey("Hair");
				InMaterial->SetMaterialType(EMaterialType::AnisotoropyKaijiyakay);
			}
		}

		//OrenNayar
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(
					0.7f,
					0.7f,
					1.4f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::OrenNayar);

				InMaterial->SetRoughness(0.7f);//�����0 ����������
			}
		}

		//
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(
					0.7f,
					0.7f,
					1.4f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::Banded);
			}
		}

		//GradualBanded
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(
					247.f / 255.f,
					150.f / 255.f,
					85.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::GradualBanded);
			}
		}

		//����Banded
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(
					213.f / 255.f,
					132.f / 255.f,
					234.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::CustomBanded);
				InMaterial->SetSpecular(XMFLOAT3(1.f, 1.f, 1.f));
				InMaterial->SetRoughness(0.6f);
			}
		}

		//back
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(
					2.f / 255.f,
					214.f / 255.f,
					17.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::Back);
				InMaterial->SetSpecular(XMFLOAT3(1.f, 1.f, 1.f));
				InMaterial->SetRoughness(0.2f);
			}
		}

		//���߿���ʾ
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
				InMaterial->SetMaterialType(EMaterialType::BaseColor);
				InMaterial->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));

			}
		}

		//�Ե���ʾ
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::PointDisplay);
				InMaterial->SetMaterialType(EMaterialType::BaseColor);
				InMaterial->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
			}
		}

		//�����編����ʾ
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::WorldNormal);
			}
		}

		//�Է�����ʾ
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 24, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::Normal);
			}
		}

		//��ʾBaseColor��ͼ1
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColorIndexKey("Wood");
				InMaterial->SetNormalIndexKey("Wood_NRM");
				InMaterial->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
				InMaterial->SetRoughness(4.f);
				InMaterial->SetMaterialType(EMaterialType::OrenNayar);
			}
		}

		//��ʾBaseColor��ͼ2
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColorIndexKey("MMOARPG.dds");
				InMaterial->SetBaseColor(XMFLOAT4(0.7f, 0.7f, 0.7f, 1.f));
				InMaterial->SetNormalIndexKey("MMOARPG_NRM");
				InMaterial->SetMaterialType(EMaterialType::OrenNayar);
			}
		}

		////��ʾBaseColor��ͼ2
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColorIndexKey("Texture'/Project/Texture/Earth.Earth'");
				InMaterial->SetBaseColor(XMFLOAT4(0.7f, 0.7f, 0.7f, 1.f));
				InMaterial->SetSpecular(XMFLOAT3(1.f, 1.f, 1.f));
				InMaterial->SetMaterialType(EMaterialType::BlinnPhong);

				InMaterial->SetFresnelF0(XMFLOAT3(0.1f, 0.1, 0.1f));
				InMaterial->SetRoughness(0.1f);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(9.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetNormalIndexKey("Wood2_Nor");
				//InMaterial->SetBaseColor("Wood2");
				InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
				InMaterial->SetRoughness(0.8f);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(15.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetNormalIndexKey("Wood2_Nor");
				//InMaterial->SetBaseColor("Wood2");
				InMaterial->SetSpecular("Wood2_SPEC");
				InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
				InMaterial->SetRoughness(0.76f);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//PBRģ��
		{
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(15.f, 2, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
				InMaterial->SetMaterialType(EMaterialType::PBR);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//͸��
		{
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_TRANSPARENT);

			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(15.f, 7, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColorIndexKey("TransparentContent");
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//������
		{
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(15.f, 12, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterial())[0])
			{
				InMaterial->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
				InMaterial->SetMaterialType(EMaterialType::BlinnPhong);

				InMaterial->SetRoughness(0.01f);
				InMaterial->SetFresnelF0(XMFLOAT3(0.5f, 0.5f, 0.5f));
			}
		}

		//if (GSky* InSky = World->CreateActorObject<GSky>())//���
		//{
		//	InSky->SetPosition(XMFLOAT3(0.f, 0.f, 0.f));
		//}

		//���ʵ��
		if (GFog* Fog = World->CreateActorObject<GFog>())
		{
			Fog->SetFogColor(XMFLOAT4(0.7f, 0.7f, 0.9f, 1.f));
			Fog->SetFogStart(10.f);
			Fog->SetFogRange(500.f);

			Fog->SetFogHeight(5000.f);
			Fog->SetFogTransparentCoefficient(0.00f);

		}

		/*if (GMesh* CylinderMesh = MeshManage->CreateCylinderMesh(1.f, 1.f, 5.f, 20, 20))
		{
			CylinderMesh->SetPosition(XMFLOAT3(1, -2, -4));
		}

		if (GMesh* ConeMesh = MeshManage->CreateConeMesh(1.f, 5.f, 20, 20))
		{
			ConeMesh->SetPosition(XMFLOAT3(-1, 1, 9));
			ConeMesh->SetRotation(fvector_3d(90.f, 1.f, 20.f));
		}*/
	}


	MeshManage->BuildMesh();

	ANALYSIS_RESULT(GraphicsCommandList->Close());

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	WaitGPUCommandQueueComplete();

	ENGINE_LOG("����post��ʼ�����");

	return 1;
}

void CDirectXRenderingEngine::UpdateCalculations(float DeltaTime, const FViewportInfo& viewport_info)
{
	MeshManage->UpdateCalculations(DeltaTime, viewport_info);
}

void CDirectXRenderingEngine::Tick(float DeltaTime)
{
	// ����¼������ڴ棬Ϊ��һ֡������׼��
	ANALYSIS_RESULT(CommandAllocator->Reset());
	// Ԥ��Ⱦ
	MeshManage->PreDraw(DeltaTime);

	// ָ���ĸ���Դ��ת����״̬����Ϊ����������buffer�������ڲ��Ͻ���
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuffer(),				// ��ǰbuffer������
		D3D12_RESOURCE_STATE_PRESENT,		// ��ǰ״̬ ��ʾ��Դ��������������Ŀ��
		D3D12_RESOURCE_STATE_RENDER_TARGET	// Ŀ��״̬ �⽫ʹ��Դ����������ȾĿ�꣬���������Ը���Դִ�г��ֲ�����д��״̬��
	);
	GraphicsCommandList->ResourceBarrier(
		1,
		&ResourceBarrierPresent
	);

	// ���ã����£��ӿ���Ϣ���ü�������Ϣ
	GraphicsCommandList->RSSetViewports(1, &ViewPortInfo);
	GraphicsCommandList->RSSetScissorRects(1, &ViewPortRect);


	// �������
	constexpr float ColorBG[] = { 0.1f, 0.105f, 0.11f, 1.0f };
	GraphicsCommandList->ClearRenderTargetView(
		GetCurrentSwapBufferView(),		// Ҫ�������ȾĿ����ͼ
		ColorBG,		// ������ɫ
		0,		// ���������������Ǻ��ӿ���صģ����ﲻ���������ã��������ר�ŵ����÷���
		nullptr
	);

	// �����Ⱥ�ģ�建����
	GraphicsCommandList->ClearDepthStencilView(
		GetCurrentDepthStencilView(),	// ����CPU�ڴ棨Ҫ��������ģ�建�����ڴ棩
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,		// �����Ⱥ�ģ�建����
		1.f,	// ��1��������ǵ���Ȼ�����������Ȼ���������Ϊ1
		0,		// ��0��������ǵ�ģ�建������ģ�建��������Ϊ0
		0,
		nullptr
	);

	// ָ����Ⱦ������������ϲ��׶Σ�������ȾĿ����ͼ
	D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView = GetCurrentDepthStencilView();
	GraphicsCommandList->OMSetRenderTargets(
		1,									// ָ����ȾĿ���� 1
		&SwapBufferView,		// ָ����ȾĿ��
		true,								// true�������Ǵ���ľ����һ���ڴ�������������ָ��
		&DepthStencilView		// �������
	);

	// ��Ⱦ
	MeshManage->Draw(DeltaTime);
	MeshManage->PostDraw(DeltaTime);

	// ���õ�ǰ������buffer״̬
	CD3DX12_RESOURCE_BARRIER ResourceBarrierRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
	GraphicsCommandList->ResourceBarrier(
		1,
		&ResourceBarrierRenderTarget
	);

	// �Դˣ�������¼����ɣ��ǵùر������б��������ύ������
	ANALYSIS_RESULT(GraphicsCommandList->Close());

	// �ύ����
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);


	// ����Buffer������
	ANALYSIS_RESULT(SwapChain->Present(0, 0));	// ��ֱͬ��0�� flagsҲ��0

	CurrentSwapBufferIndex = !static_cast<bool>(CurrentSwapBufferIndex);	// ��Ϊ���ǵĽ�������������ֻ��2���������������Լ򵥵�ʹ�����ַ������� ��0,1��

	// CPU�ȴ�GPUִ�н��
	WaitGPUCommandQueueComplete();
}

int CDirectXRenderingEngine::PreExit()
{
	return 0;
}

int CDirectXRenderingEngine::Exit()
{
	return 0;
}

int CDirectXRenderingEngine::PostExit()
{
	return 0;
}

ID3D12Resource* CDirectXRenderingEngine::GetCurrentSwapBuffer() const
{
	return SwapChainBuffer[CurrentSwapBufferIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentSwapBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		RTVHeap->GetCPUDescriptorHandleForHeapStart(),	// ͨ��RTV��ջ��ȡ��ǰCPU������
		CurrentSwapBufferIndex,	// ���뵱ǰbuffer������Ҳ���Ƕ�ջ�ڴ����棬����Ҫ��ȡ������һ����RTV�����������������ڴ��ǰ��
		RTVDescriptorSize		// ���뵱ǰRTV�����Ĵ�С
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentDepthStencilView() const
{
	return DSVHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT CDirectXRenderingEngine::GetDXGISampleCount() const
{
	return bMSAA4XEnabled ? 4 : 1;
}

UINT CDirectXRenderingEngine::GetDXGISampleQuality() const
{
	return bMSAA4XEnabled ? (M4XNumQualityLevels - 1) : 0;
}

CMeshManager* CDirectXRenderingEngine::GetMeshManage()
{
	return MeshManage;
}

CLightManager* CDirectXRenderingEngine::GetLightManager()
{
	return LightManager;
}

void CDirectXRenderingEngine::WaitGPUCommandQueueComplete()
{
	CurrentFenceIndex++;

	// ��GPU�����µĸ���� �ȴ�GPU�������ź� �������źţ�
	ANALYSIS_RESULT(CommandQueue->Signal(Fence.Get(), CurrentFenceIndex));

	if (Fence->GetCompletedValue() < CurrentFenceIndex)
	{
		// �������ߴ�һ���¼��ں˶��󣬲����ظ��ں˶���ľ��
		// ����1 SECURITY_ATTRIBUTES
		// ����2 �¼�����null��ʾ�����¼�
		// ����3
		// CREATE_EVENT_INITIAL_SET		0x00000002	��ʾ���ǵĶ���ĳ�ʼ���Ƿ񱻴����ˣ�FALSE��ʾδ����
		// CREATE_EVENT_MANUAL_RESET	0x00000001	��ʾ����¼��������Ҫ��ResetEvents���ã��㲻���������־�Ļ������ǵ�ϵͳ�ں˾ͻ����Ĭ������
		// ����4 �¼�����ķ���Ȩ��
		HANDLE EventEX = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);

		// �����������Ѿ����ú��ź��ˣ���ô�������źŴ����ͻ�֪ͨ����¼����������д���ﵽ��Ч������
		// �������������õ�GPU�źţ���GPU��ɺ󣬾ͻ�֪ͨ���¼�EventX���Ӷ���CPU֪��GPU������ִ�����
		// GPU��ɺ��֪ͨ���ǵ�ǰ���¼����EventEX
		ANALYSIS_RESULT(Fence->SetEventOnCompletion(CurrentFenceIndex, EventEX));

		// �ȴ��ź��¼��������ȴ�ʱ��Ϊ���޵ȴ� ���ȴ�GPU��
		// ������������̣߳���ֹCPU������GPU�ύ����
		WaitForSingleObject(EventEX, INFINITE);

		// ����¼�����ˣ����ͷŵ���ǰ�¼�����ô���Ǿͻ���������յ�����¼����߳̾ͻ������ﱻ��ԭ�������»��ѣ�Ȼ�����Ǿͽ���ǰ�¼��رռ���
		CloseHandle(EventEX);
	}
}

bool CDirectXRenderingEngine::InitDirect3D()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����pix����
	// PIXLoadLatestWinPixGpuCapturerLibrary();


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����DX12���Բ� debug
	ComPtr<ID3D12Debug> D3d12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3d12Debug))))
	{
		D3d12Debug->EnableDebugLayer();		// �������Բ�
	}

	// HRESULT
	// S_OK					0x00000000	ִ�гɹ�
	// E_UNEXPECTED			0x8000FFFF	�����ʧ��
	// E_NOTIMPL			0x80004001	δʵ��
	// E_OUTOFMEMORY		0x8007000E	δ�ܷ��������ڴ�
	// E_INVALIDARG			0x80070057	һ�����߶��������Ч
	// E_NOINTERFACE		0x80004002	��֧�ִ˽ӿ�
	// E_POINTER			0x80004003	��Чָ��
	// E_HANDLE				0x80070006	��Ч���
	// E_ABORT				0x80004004	������ֹ
	// E_ACCESSDENIED		0x80070005	һ��ķ��ʱ��ܾ�����

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����DGI�豸����

	// ����DGIʵ��
	ANALYSIS_RESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGiFactory)));

	// ��������
	//	IUnknown * pAdapter,          // ����Ҫʹ�õ�������(�豸)ʵ��ָ�룬���Ϊnullptr��ʹ��Ĭ��������
	//	D3D_FEATURE_LEVEL          MinimumFeatureLevel,// ���֧�ֵ����Լ���
	//	REFIID                     riid,               // ϣ��ͨ���˺����������豸�ӿ����� ID
		// D3D_FEATUREI_LEVEL_9_ 1 Ŀ�깦�ܼ���֧��Direct3D 9.1  ����shader model 2.
		// D3D_FEATURE_LEVEL_9_2   Ŀ�깦�ܼ���֧��Direct3D 9.2  ����shader model 2.
		// D3D_FEATURE_LEVEL_9_3   Ŀ�깦�ܼ���֧��Direct3D 9.3  ����shader model 3.
		// D3D_FEATURE_LEVEL_10_0  Ŀ�깦�ܼ���֧��Direct3D 10.0 ����shader model 4.
		// D3D_FEATURE_LEVEL_10_1  Ŀ�깦�ܼ���֧��Direct3D 10.1 ����shader model 4.
		// D3D_FEATURE_LEVEL_11_0  Ŀ�깦�ܼ���֧��Direct3D 11.0 ����shader model 5 .

	//	void** ppvDevice        // ���ص�ָ���豸�ӿڵ�ָ���ָ��
	/*pAdapter��һ��ָ��IDXGIAdapter1����IDXGIAdapter4��ָ�룬��ʾ����ϣ��ʹ�õ����������豸��ʵ��������ò���Ϊnullptr����Direct3D 12�᳢��ʹ��Ĭ�ϵ���������

		MinimumFeatureLevel�����֧�ֵ����Լ��𣬱�ʾӦ�ó�����Ҫʹ�õ�Ӳ������ˮƽ���ò�����ֵӦ����ö������D3D_FEATURE_LEVEL�е�һ����Ա������D3D_FEATURE_LEVEL_11_0��

		riid��һ��Ψһ��ʶ�豸�ӿ����͵�GUID���ò���ͨ����������ΪIID_ID3D12Device����ʾ������Ҫ����һ��ID3D12Device����

		ppvDevice�����ص�ָ���豸�ӿڵ�ָ���ָ�롣�ں����ɹ�ִ�к󣬸�ָ�뽫ָ��һ��ID3D12Device�ӿڵ�ʵ��*/
	HRESULT D3dDeviceResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		// ʧ�ܣ�Ӳ������������ʧ�ܣ�
		// Ӳ������������ʧ�ܣ���ô����Ͳ������������
		// warp �߼���դ��ƽ̨

		// ����������
		ComPtr<IDXGIAdapter> WARPAdapter;
		// ö��������
		ANALYSIS_RESULT(DXGiFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		// ����������ʹ�����ģ���������������������
		ANALYSIS_RESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	// ����fence(����Χ����ΪCPU��GPUͬ����׼��
	// ���������������£�
	// InitialValue��Χ����ʼֵ����ʾΧ��״̬����GPUִ�������б�ʱ����д��һ���ź�ֵ��Χ���У�CPU����ͨ������ֵ��ȷ��GPU�Ƿ��Ѿ��������Ⱦ������
	// Flags����־λ���̶�ΪD3D12_FENCE_FLAG_NONE��
	// riid��Ψһ��ʶΧ���ӿ����͵�GUID���̶�ΪIID_ID3D12Fence��
	// ppFence�����ص�ָ��Χ���ӿڵ�ָ���ָ�롣�ں����ɹ�ִ�к󣬸�ָ�뽫ָ��һ��ID3D12Fence�ӿڵ�ʵ����
	ANALYSIS_RESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �����豸���������ǾͿ��Դ������ǵ����������
	// ��ʼ���������

	// �����������
	// D3D12_COMMAND_QUEUE_DESC��DirectX 12�е�һ���ṹ�����ͣ��������������������ʱ�����ԺͲ��������������¼�����Ա������
	// Type��������е����ͣ�������D3D12_COMMAND_LIST_TYPE_DIRECT��D3D12_COMMAND_LIST_TYPE_BUNDLE��D3D12_COMMAND_LIST_TYPE_COMPUTE��D3D12_COMMAND_LIST_TYPE_COPY֮һ��
	// Priority��������е����ȼ���������D3D12_COMMAND_QUEUE_PRIORITY_NORMAL��D3D12_COMMAND_QUEUE_PRIORITY_HIGH֮һ��
	// Flags��������еı�־λ��������D3D12_COMMAND_QUEUE_FLAG_NONE��D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT��D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_IDLE_DEPENDENCY֮һ��
	// NodeMask��CPU�ڵ����룬��ʾ��������п�������ЩCPU�ڵ��ϵ��߳�ʹ�á������Ҫ�ڶ��CPU�ڵ��ϲ���ִ�У�����Ҫʹ�ö��������С�
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};		// ��������
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;				// ָ�������б������(ֱ�����ֱ�ӿ�����GPU��ִ�е�����)
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;				// ָ��������еı�־������ʱ��
	ANALYSIS_RESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

	// ����������
	ANALYSIS_RESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf())
	));

	// ���������б�
	ANALYSIS_RESULT(D3dDevice->CreateCommandList(
		0,															// Ĭ��һ��GPU
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,	// ֱ��GPUִ������
		CommandAllocator.Get(),										// ����ǰ�����б��������������
		nullptr,													// ��Ҫ���뵱ǰ����״̬
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())
	));

	// �����������б���Ҫ���ǽ���ر�
	GraphicsCommandList->Close();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ö��ز���

	// ��ѯָ��Ӳ������Դ��ʽ֧�ֵĶ��ز�������ˮƽ
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.SampleCount = 4;	// ��ѯ������4
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;	// ����������������ΪĬ�ϣ���֧���κ�ѡ��
	QualityLevels.NumQualityLevels = 0;
	QualityLevels.Format = BackBufferFormat;
	// ����CheckFeatureSupport��ѯ����Ŀǰ���豸�����Ƿ�֧���������õ���Щ��������
	ANALYSIS_RESULT(D3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,	// ָ���������ǣ����ǵ�ǰ�����ļ���
		&QualityLevels,								// ������������
		sizeof(QualityLevels)						// ����������С
	));

	// ������������
	M4XNumQualityLevels = QualityLevels.NumQualityLevels;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������

	// �������������֤���Ǵ���һ���µĽ�����
	SwapChain.Reset();

	// ��������������
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	// buffer����
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;					// ָ����
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;					// ָ����
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;	// ָ��ˢ���ʣ����ӣ�
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;													// ָ��ˢ���ʣ���ĸ��
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;						// ָ����������buff����������������һ�㶼��˫����͹��ˣ�
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		// ָ��ɨ���ߵ���ʾ˳���³�����ģʽ��
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			// ����̨������(�������Դ)������ȾĿ�꣬�����л�����Ⱦ��������������ʹ�÷�ʽ��Ҳ��Ĭ��ֵ
	// ��������
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;		// �����ʽ
	// ��������
	SwapChainDesc.OutputWindow = MainWindowsHandle;		// ָ�����ھ��
	SwapChainDesc.Windowed = true;						// �Դ���ģʽ����(false��ȫ����
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;	// ǿ��ˢ��ģʽ����ÿ�γ�����֡ʱ�����ǰ�������������������ݡ���������Ľ�����ʽ��Ҳ��Ĭ��ֵ��
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // ������ʾģʽ�л� (�������������ɵ��л���ʾ���ڣ���ȫ����ȫ�����봰�ھʹ��ڣ�
	// ��������(���ز������ã�
	SwapChainDesc.SampleDesc.Count = GetDXGISampleCount();	// ���ò���������Ĳ������������ж϶��ز����Ƿ����������������ô��ֵΪ4��Ĭ�Ͼ��ǿ���4�ز������������0
	SwapChainDesc.SampleDesc.Quality = GetDXGISampleQuality();		// ���ò�����������������,ͶӰ��Ҫ�ж��Ƿ������ز����������������ֵΪ����֮ǰ�趨�Ĳ�������-1������Ϊ0

	// ����������
	ANALYSIS_RESULT(DXGiFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf()));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// ��Դ������

	// RTV
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc; // ���� �����������ѣ�Descriptor Heap�������Ժ�������Ϣ RTV
	// ��������������
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount + 6;						// ��������������������Ϊ2������Ϊ����ʹ��˫���壨ǰ̨����������̨��������������������Ⱦ��ͼ��������Ҫ����RTV�� ����������2����+6��ʾCubeMap��6����Ľ�������
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// ָ������������-��ȾĿ����ͼ
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// ָ��������Ĭ���÷�
	RTVDescriptorHeapDesc.NodeMask = 0;								// ָ���������ѵĽڵ����룬���ڶ��GPU�ڵ�֮���ͨ����ͬ�� (0��ʾ�����ã�ʹ��Ĭ��GPU�ڵ㣩
	// ���ú����ԾͿ��Դ�����
	ANALYSIS_RESULT(D3dDevice->CreateDescriptorHeap(&RTVDescriptorHeapDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));		// ʹ���豸��������RTV��������


	// DSV
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc; // �����������ѣ�Descriptor Heap�������Ժ�������Ϣ DSV
	// ��������������
	DSVDescriptorHeapDesc.NumDescriptors = 1 + 1;					// ��������������Ȼ�����ֻ��Ҫһ���͹��ˣ���Ȼ�����Ϣ�ǿ��Ը��õģ���������һ������ʾCubeMap�������Ϣ��
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	// ָ������������-���ģ����ͼ
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// ָ��������Ĭ���÷�
	DSVDescriptorHeapDesc.NodeMask = 0;								// ָ���������ѵĽڵ����룬���ڶ��GPU�ڵ�֮���ͨ����ͬ�� (0��ʾ�����ã�ʹ��Ĭ��GPU�ڵ㣩
	// ���ú����ԾͿ��Դ�����
	ANALYSIS_RESULT(D3dDevice->CreateDescriptorHeap(&DSVDescriptorHeapDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf())));		// ʹ���豸��������DSV��������


	return true;
}

void CDirectXRenderingEngine::PostInitDirect3D()
{
	// ��Ϊ������ǰ���д�����ص����ݣ�����������ִ�еȴ���ͬ����
	// CPU�ȴ�GPUִ�н��
	WaitGPUCommandQueueComplete();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����̨�������󶨵���Ⱦ��ˮ��

	// �����ǵ���ȾĿ�껺�����reset
	for (auto& buffer : SwapChainBuffer)
	{
		buffer.Reset();
	}
	// ���ģ��ҲҪ����Reset
	DepthStencilBuffer.Reset();

	// �����ǵĽ��������ô�С
	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,		// ����������
		FEngineRenderConfig::GetRenderConfig()->ScreenWidth,		// ��Ļ���
		FEngineRenderConfig::GetRenderConfig()->ScreenHeight,		// ��Ļ�߶�
		BackBufferFormat,											// �����ʽ
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH						// ��������ǣ����������ǣ����Ǿ���ͨ��ResetTarget���ڴ���ģʽ��ȫ��ģʽ֮���л����������л���ʱ��������ʾģʽ���Ǹ����ڵķֱ��ʾͻ��������Ӧƥ�䣩
	);

	// �����ǵ�ǰ����Դ�󶨵���Ⱦ��ˮ����

	// ��ȡ��ǰRTV�������Ĵ�С
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// �����Ǵ������������Ժ���������Ҫһ���Ѿ�������з���
	CD3DX12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, HeapHandle);
		// CreateRenderTargetView ������Ⱦ��ͼ
		// ����1��ָ����Ⱦ������
		// ����2��ָ����̨��������ʽ����Ϊ�����ڴ��������ѵ�ʱ����Ѿ�ָ���˸�ʽ�ˣ��������ﴫ��nullptr����ʾĬ��
		// ����3�����뵱ǰRTV��ȾĿ����ͼ�ľ��

		// ��������Ⱦ��ͼ����Ҫ��һ��ƫ�ƵĲ����������ǵ�ǰ������ƫ�Ƶ���һ������������Ϊ������������������һ��ǰ̨��������һ����̨��������ƫ����Ϊһ��RTV�Ĵ�С��
		// HeapHandle.ptr += RTVDescriptorSize;
		// ���ʹ����΢���ṩ��d3dx12.h�Ǹ��ӿڣ���ô����Ͳ����Լ�����ƫ����
		HeapHandle.Offset(1, RTVDescriptorSize);

	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��ʼ���ύ��Դ

	// ��Դ����
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;		// ָ����Դ���
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;		// ָ����Դ�߶�
	ResourceDesc.Alignment = 0;														// ���뷽ʽ
	ResourceDesc.MipLevels = 1;														// ָ��mipmap�ļ���
	ResourceDesc.DepthOrArraySize = 1;												// ָ����ȣ�ע�⣬�������ʹ�õ���3D��ô���ָ���ĺ��������ȣ������1D����2D����ָ����Ĵ�С
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;					// ָ����Դά��,�������Դ���ԣ����ﶨ��Ϊ2D����

	ResourceDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;							// ָ��������
	ResourceDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XNumQualityLevels - 1) : 0;	// ָ����������
	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;								// ָ�������ʽ��ÿ�����صĴ洢��ʽ��
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;					// ָ����ǣ�����Ϊ��ֵ��ʾ�����������ģ�壨������DX�����ǵ�ǰ�����Դ���������ģ�建��ģ�
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;								// ָ���ύ��Դ�����з�ʽ������ָ��ΪĬ�ϣ�δ֪���з�ʽ

	// ���ģʽ
	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;			// ָ��ģ������������٣���������1��
	ClearValue.DepthStencil.Stencil = 0;			// ָ��ģ�壨ģ���������٣�����0��
	ClearValue.Format = DepthStencilFormat;			// ָ����ʽ

	CD3DX12_HEAP_PROPERTIES Properties(D3D12_HEAP_TYPE_DEFAULT);	// ָ�������ͣ�����ΪĬ���ύ��Դ��

	// �����ύ��Դ
	D3dDevice->CreateCommittedResource(
		&Properties,										// ָ�������ͣ�����ΪĬ���ύ��Դ��
		D3D12_HEAP_FLAG_NONE,								// ָ���Ѷ�������ʹ�������־����Ĭ���ύ��Դ��
		&ResourceDesc,										// ָ����Դ����
		D3D12_RESOURCE_STATE_COMMON,						// ָ����ǩ����ʾ�ύ��Դ�ĳ�ʼ״̬����������Ϊ����״̬�������Ա��κ����͵Ĳ�����ʹ��
		&ClearValue,										// ָ���Ż�ֵ(ָ��Ҫ������ύ��Դֵ��
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf())
	);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ģ�建����

	// ��Ȼ�������ͼ����
	D3D12_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	DepthStencilViewDesc.Format = DepthStencilFormat;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;							// ָ���������������������������������ָ����һ��Mipmap��
	DepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;		// ָ�����ģ�建������ά�ȣ�2D����,ָ��������Ҫ���ȥ�����������ģ�建���е���Դ
	DepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;						// ָ����ͼ���ã�����ָ��Ĭ����ͼ

	// ʵ�������ģ�建����
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DepthStencilViewDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///�󶨵������б�

	// ͬ����ǰ��Դ״̬
	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		DepthStencilBuffer.Get(),									// ָ��ǰ���ģ�建�������ָ�루ָ����Դ��ָ�룩
		D3D12_RESOURCE_STATE_COMMON,								// ������Դ״̬ǰ����һ��ͨ�õ���Դ ����Դ�ĵ�ǰ״̬��
		D3D12_RESOURCE_STATE_DEPTH_WRITE);					// ������Դ״̬�� ��һ�� ���д ����Դ ����Դ��Ŀ��״̬��
	GraphicsCommandList->ResourceBarrier(
		1,							// ָ����Դ�ύ����
		&Barrier
	);

	// ͬ������Դ״̬�����Դ���йر�
	GraphicsCommandList->Close();

	// ����һ��ָ�����飬���ڴ洢����ָ���б���������Դ�źܶ�������
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };

	// ʹ�ö����ύ����
	// _countof��d3d12x.h���������㾲̬��������Ԫ�صĸ�����sizeof�����������ֽ�����
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��Ⱦ�ӿ�����(��Щ���ûḲ��ԭ�ȵ�Windows����)

	// �����ӿڳߴ�
	// DirectX������ϵͳ��OpenGL�ǲ�һ���ģ�DX������ϵԭ��λ�ڴ��ڵ����ģ�OpenGL������ԭ��������Ļ�����½�
	ViewPortInfo.TopLeftX = 0;
	ViewPortInfo.TopLeftY = 0;
	ViewPortInfo.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewPortInfo.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ViewPortInfo.MinDepth = 0.f;	// ��С���
	ViewPortInfo.MaxDepth = 1.f;	// ������

	// �����ü�����
	ViewPortRect.top = 0;
	ViewPortRect.left = 0;
	ViewPortRect.right = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewPortRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;

	// CPU�ȴ�GPUִ�н��
	WaitGPUCommandQueueComplete();
}

