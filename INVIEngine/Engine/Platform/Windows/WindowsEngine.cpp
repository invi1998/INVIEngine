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
	// 处理日志
	constexpr char LogPath[] = "../log";
	init_log_system(LogPath);
	ENGINE_LOG("Log init.");

	// 处理命令


	ENGINE_LOG("引擎预初始化完成");

	return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	// 处理视口
	InitWindows(InParameters);

	RenderingEngine->SetMainWindowsHandle(MainWindowsHandle);

	RenderingEngine->Init(InParameters);

	World = CreateObject<CWorld>(new CWorld());
	RenderingEngine->World = World;

	ENGINE_LOG("引擎初始化完成");

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
	// 绑定窗口参数
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);	// 该对象实际占用的内存
	WindowsClass.cbClsExtra = 0;				// 是否需要额外空间 表示窗口类额外内存大小，即每个窗口创建时将分配给该窗口类的额外内存大小
	WindowsClass.cbWndExtra = 0;				// 是否需要额外内存 表示窗口实例额外内存大小，即为每个窗口实例分配的额外内存大小
	// cbClsExtra是用于定义一个窗口类的额外数据的大小，而cbWndExtra是用于定义一个窗口实例的额外数据的大小。通常，开发者可以使用这些额外的内存来存储一些自定义数据或指针，以便在处理消息和其他操作时进行访问。
	WindowsClass.hbrBackground = nullptr;		// 用于指定窗口背景所使用的画刷句柄，如果没有就是设置GDI擦除
	WindowsClass.hCursor = LoadCursor(nullptr, IDC_ARROW);	// 设置箭头光标
	WindowsClass.hIcon = nullptr;				// (我们应用程序放在磁盘的一个图标）
	WindowsClass.hIconSm = nullptr;				// 应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance;	// 窗口实例
	WindowsClass.lpszClassName = L"INVIEngine";	// 窗口名字
	WindowsClass.lpszMenuName = nullptr;		// 菜单名字
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;	// 如何绘制当前窗口（垂直和水平绘制）
	WindowsClass.lpfnWndProc = EngineWidowProc;		// 消息处理函数

	// 注册窗口
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);

	if (!RegisterAtom)
	{
		MessageBox(NULL, L"Register fail,", L"Error", MB_OK);
	}

	// 视口，视口风格，没有菜单
	RECT Rect = { 0, 0, FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight };
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int windowWidth = Rect.right - Rect.left;
	int windowHeight = Rect.bottom - Rect.top;

	MainWindowsHandle = CreateWindowEx(
		NULL,						// 窗口额外风格
		L"INVIEngine",				// 窗口名称
		L"INVI Engine",				// 显示到标题栏上的窗口名称
		WS_OVERLAPPEDWINDOW,		// 视口风格
		CW_USEDEFAULT, CW_USEDEFAULT,		// 窗口坐标，设置为CW_USEDEFAULT让窗口自适应大小并且显示在屏幕中心
		windowWidth,				// 窗口宽度
		windowHeight,				// 窗口高度
		nullptr,					// 父窗口句柄
		nullptr,					// 菜单句柄
		InParameters.HInstance,		// 窗口实例
		NULL						// 传递进窗口的额外参数
	);

	if (!MainWindowsHandle)
	{
		// 窗口创建失败
		MessageBox(0, L"窗口创建失败", 0, 0);
		return false;
	}

	// 显示窗口
	ShowWindow(MainWindowsHandle, SW_SHOW);

	// 刷新窗口
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

