#include "WindowsEngine.h"

#include "WindowsMessageProcessing.h"
#include "Config/EngineRenderConfig.h"
#include "Core/Camera.h"
#include "Core/QuaternionCamera.h"
#include "Core/World.h"
#include "Debug/EngineDebug.h"
#include "Mesh/BoxMesh.h"
#include "Mesh/CylinderMesh.h"
#include "Mesh/ConeMesh.h"
#include "Mesh/PlaneMesh.h"
#include "Mesh/CustomMesh.h"
#include "Mesh/SphereMesh.h"
#include "Rendering/Core/Rendering.h"
#include "Rendering/Engine/DirectX/DirectX12RenderingEngine.h"

CWindowsEngine::CWindowsEngine()
{
	RenderingEngine = new CDirectX12RenderingEngine();
	
}

CWindowsEngine::~CWindowsEngine()
{
	delete RenderingEngine;
}

int CWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// ������־
	constexpr char LogPath[] = "../log";
	init_log_system(LogPath);
	ENGINE_LOG("Log init.");

	// ��������


	ENGINE_LOG("����Ԥ��ʼ�����");

	return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	// �����ӿ�
	InitWindows(InParameters);

	RenderingEngine->SetMainWindowsHandle(MainWindowsHandle);

	RenderingEngine->Init(InParameters);

	World = CreateObject<CWorld>(new CWorld());
	RenderingEngine->World = World;

	ENGINE_LOG("�����ʼ�����");

	return 0;
}

int CWindowsEngine::PostInit()
{

	RenderingEngine->PostInit();

	for (auto& Tmp : GObjects)
	{
		Tmp->BeginInit();
	}

	

	return 0;
}

void CWindowsEngine::Tick(float DeltaTime)
{
	for (auto& temp : GObjects)
	{
		if (temp->IsTick())
		{
			temp->Tick(DeltaTime);
		}
	}

	if (World)
	{
		if (World->GetCamera())
		{
			FViewportInfo viewPortInfo;

			viewPortInfo.ViewMatrix = World->GetQuaternionCamera()->GetViewMatrixFx4();
			viewPortInfo.ProjectionMatrix = World->GetQuaternionCamera()->GetProjectionMatrixFx4();
			XMStoreFloat4(&viewPortInfo.CameraPosition, World->GetQuaternionCamera()->GetPosition());

			RenderingEngine->UpdateCalculations(DeltaTime, viewPortInfo);

			RenderingEngine->Tick(DeltaTime);
		}
		
	}
	
}

int CWindowsEngine::PreExit()
{
	RenderingEngine->PreExit();

	return 0;
}

int CWindowsEngine::Exit()
{
	RenderingEngine->Exit();

	return 0;
}

int CWindowsEngine::PostExit()
{
	FEngineRenderConfig::Destroy();

	RenderingEngine->PostExit();

	return 0;
}

bool CWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	// �󶨴��ڲ���
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);	// �ö���ʵ��ռ�õ��ڴ�
	WindowsClass.cbClsExtra = 0;				// �Ƿ���Ҫ����ռ� ��ʾ����������ڴ��С����ÿ�����ڴ���ʱ��������ô�����Ķ����ڴ��С
	WindowsClass.cbWndExtra = 0;				// �Ƿ���Ҫ�����ڴ� ��ʾ����ʵ�������ڴ��С����Ϊÿ������ʵ������Ķ����ڴ��С
	// cbClsExtra�����ڶ���һ��������Ķ������ݵĴ�С����cbWndExtra�����ڶ���һ������ʵ���Ķ������ݵĴ�С��ͨ���������߿���ʹ����Щ������ڴ����洢һЩ�Զ������ݻ�ָ�룬�Ա��ڴ�����Ϣ����������ʱ���з��ʡ�
	WindowsClass.hbrBackground = nullptr;		// ����ָ�����ڱ�����ʹ�õĻ�ˢ��������û�о�������GDI����
	WindowsClass.hCursor = LoadCursor(nullptr, IDC_ARROW);	// ���ü�ͷ���
	WindowsClass.hIcon = nullptr;				// (����Ӧ�ó�����ڴ��̵�һ��ͼ�꣩
	WindowsClass.hIconSm = nullptr;				// Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	WindowsClass.hInstance = InParameters.HInstance;	// ����ʵ��
	WindowsClass.lpszClassName = L"INVIEngine";	// ��������
	WindowsClass.lpszMenuName = nullptr;		// �˵�����
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;	// ��λ��Ƶ�ǰ���ڣ���ֱ��ˮƽ���ƣ�
	WindowsClass.lpfnWndProc = EngineWidowProc;		// ��Ϣ������

	// ע�ᴰ��
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);

	if (!RegisterAtom)
	{
		MessageBox(NULL, L"Register fail,", L"Error", MB_OK);
	}

	// �ӿڣ��ӿڷ��û�в˵�
	RECT Rect = { 0, 0, FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight };
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int windowWidth = Rect.right - Rect.left;
	int windowHeight = Rect.bottom - Rect.top;

	MainWindowsHandle = CreateWindowEx(
		NULL,						// ���ڶ�����
		L"INVIEngine",				// ��������
		L"INVI Engine",				// ��ʾ���������ϵĴ�������
		WS_OVERLAPPEDWINDOW,		// �ӿڷ��
		CW_USEDEFAULT, CW_USEDEFAULT,		// �������꣬����ΪCW_USEDEFAULT�ô�������Ӧ��С������ʾ����Ļ����
		windowWidth,				// ���ڿ��
		windowHeight,				// ���ڸ߶�
		nullptr,					// �����ھ��
		nullptr,					// �˵����
		InParameters.HInstance,		// ����ʵ��
		NULL						// ���ݽ����ڵĶ������
	);

	if (!MainWindowsHandle)
	{
		// ���ڴ���ʧ��
		MessageBox(0, L"���ڴ���ʧ��", 0, 0);
		return false;
	}

	// ��ʾ����
	ShowWindow(MainWindowsHandle, SW_SHOW);

	// ˢ�´���
	UpdateWindow(MainWindowsHandle);
}

CMeshManager* CWindowsEngine::GetMeshManage()
{
	return RenderingEngine->GetMeshManage();
}

CWorld* CWindowsEngine::GetWorld()
{
	return World;
}

