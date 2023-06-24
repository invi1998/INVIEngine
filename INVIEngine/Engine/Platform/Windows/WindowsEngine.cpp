#include "WindowsEngine.h"

#include "WindowsMessageProcessing.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// 初始化日志系统
	const char LogPath[] = "../log";
	init_log_system(LogPath);
	ENGINE_LOG("日志初始化完毕")

	// 处理命令


	// 处理视口
	if (InitWindows(InParameters))
	{
		
	}

	ENGINE_LOG("引擎预初始化完成")

	return 0;
}

int FWindowsEngine::Init()
{
	ENGINE_LOG("引擎初始化完成")
	return 0;
}

int FWindowsEngine::PostInit()
{
	ENGINE_LOG("引擎Post初始化完成")
	return 0;
}

void FWindowsEngine::Tick()
{
	
}

int FWindowsEngine::PreExit()
{
	ENGINE_LOG("引擎预退出完成")
	return 0;
}

int FWindowsEngine::Exit()
{
	ENGINE_LOG("引擎退出完成")
	return 0;
}

int FWindowsEngine::PostExit()
{
	ENGINE_LOG("引擎退出post完成")
	return 0;
}

bool FWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
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
		ENGINE_LOG_ERROR("窗口注册失败")
		MessageBox(NULL, L"windows class 注册失败,", L"Error", MB_OK);
	}

	// 视口，视口风格，没有菜单
	RECT Rect = { 0, 0, 1280, 720 };
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int windowWidth = Rect.right - Rect.left;
	int windowHeight = Rect.bottom - Rect.top;

	HWND hwnd = CreateWindowEx(
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

	if (!hwnd)
	{
		// 窗口创建失败
		ENGINE_LOG_ERROR("窗口创建失败")
		MessageBox(0, L"窗口创建失败", 0, 0);
		return false;
	}

	// 显示窗口
	ShowWindow(hwnd, SW_SHOW);

	// 刷新窗口
	UpdateWindow(hwnd);

	ENGINE_LOG("窗口初始化完成")
}


