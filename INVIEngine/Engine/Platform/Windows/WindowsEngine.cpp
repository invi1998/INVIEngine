#include "WindowsEngine.h"

#include "WindowsMessageProcessing.h"
#include "Debug/EngineDebug.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// 处理命令


	// 处理视口
	if (InitWindows(InParameters))
	{
		
	}


	if (InitDirect3D())
	{
		
	}



	return 0;
}

int FWindowsEngine::Init()
{
	return 0;
}

int FWindowsEngine::PostInit()
{
	return 0;
}

void FWindowsEngine::Tick()
{
	
}

int FWindowsEngine::PreExit()
{
	return 0;
}

int FWindowsEngine::Exit()
{
	return 0;
}

int FWindowsEngine::PostExit()
{
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
		MessageBox(NULL, L"Register fail,", L"Error", MB_OK);
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
		MessageBox(0, L"窗口创建失败", 0, 0);
		return false;
	}

	// 显示窗口
	ShowWindow(hwnd, SW_SHOW);

	// 刷新窗口
	UpdateWindow(hwnd);
}

bool FWindowsEngine::InitDirect3D()
{
	// HRESULT
	// S_OK					0x00000000	执行成功
	// E_UNEXPECTED			0x8000FFFF	意外的失败
	// E_NOTIMPL			0x80004001	未实现
	// E_OUTOFMEMORY		0x8007000E	未能分配所需内存
	// E_INVALIDARG			0x80070057	一个或者多个参数无效
	// E_NOINTERFACE		0x80004002	不支持此接口
	// E_POINTER			0x80004003	无效指针
	// E_HANDLE				0x80070006	无效句柄
	// E_ABORT				0x80004004	操作终止
	// E_ACCESSDENIED		0x80070005	一般的访问被拒绝错误

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 创建DGI设备驱动

	// 创建DGI实例
	ANALYSIS_RESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGiFactory)));

	// 创建驱动
	//	IUnknown * pAdapter,          // 程序要使用的适配器(设备)实例指针，如果为nullptr则使用默认适配器
	//	D3D_FEATURE_LEVEL          MinimumFeatureLevel,// 最低支持的特性级别
	//	REFIID                     riid,               // 希望通过此函数创建的设备接口类型 ID
		// D3D_FEATUREI_LEVEL_9_ 1 目标功能级别支持Direct3D 9.1  包含shader model 2.
		// D3D_FEATURE_LEVEL_9_2   目标功能级别支持Direct3D 9.2  包含shader model 2.
		// D3D_FEATURE_LEVEL_9_3   目标功能级别支持Direct3D 9.3  包含shader model 3.
		// D3D_FEATURE_LEVEL_10_0  目标功能级别支持Direct3D 10.0 包含shader model 4.
		// D3D_FEATURE_LEVEL_10_1  目标功能级别支持Direct3D 10.1 包含shader model 4.
		// D3D_FEATURE_LEVEL_11_0  目标功能级别支持Direct3D 11.0 包含shader model 5 .

	//	void** ppvDevice        // 返回的指向设备接口的指针的指针
	/*pAdapter：一个指向IDXGIAdapter1或者IDXGIAdapter4的指针，表示程序希望使用的适配器（设备）实例。如果该参数为nullptr，则Direct3D 12会尝试使用默认的适配器。

		MinimumFeatureLevel：最低支持的特性级别，表示应用程序需要使用的硬件特性水平。该参数的值应该是枚举类型D3D_FEATURE_LEVEL中的一个成员，例如D3D_FEATURE_LEVEL_11_0。

		riid：一个唯一标识设备接口类型的GUID。该参数通常可以设置为IID_ID3D12Device，表示程序想要创建一个ID3D12Device对象。

		ppvDevice：返回的指向设备接口的指针的指针。在函数成功执行后，该指针将指向一个ID3D12Device接口的实例*/
	HRESULT D3dDeviceResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		// 失败（硬件适配器创建失败）
		// 硬件适配器创建失败，那么这里就采用软件适配器
		// warp 高级光栅化平台

		// 创建适配器
		ComPtr<IDXGIAdapter> WARPAdapter;
		// 枚举适配器
		ANALYSIS_RESULT(DXGiFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		// 创建驱动（使用软件模拟的适配器来创建驱动）
		ANALYSIS_RESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	// 创建fence(创建围栏）为CPU和GPU同步做准备
	// 各个参数含义如下：
	// InitialValue：围栏初始值，表示围栏状态。当GPU执行命令列表时，会写入一个信号值到围栏中，CPU可以通过检查该值来确定GPU是否已经完成了渲染操作。
	// Flags：标志位，固定为D3D12_FENCE_FLAG_NONE。
	// riid：唯一标识围栏接口类型的GUID，固定为IID_ID3D12Fence。
	// ppFence：返回的指向围栏接口的指针的指针。在函数成功执行后，该指针将指向一个ID3D12Fence接口的实例。
	ANALYSIS_RESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 有了设备驱动后，我们就可以创建我们的命令对象了
	// 初始化命令对象

	// 创建命令队列
	// D3D12_COMMAND_QUEUE_DESC是DirectX 12中的一个结构体类型，用于描述创建命令队列时的属性和参数。它包含以下几个成员变量：
	// Type：命令队列的类型，可以是D3D12_COMMAND_LIST_TYPE_DIRECT、D3D12_COMMAND_LIST_TYPE_BUNDLE、D3D12_COMMAND_LIST_TYPE_COMPUTE或D3D12_COMMAND_LIST_TYPE_COPY之一。
	// Priority：命令队列的优先级，可以是D3D12_COMMAND_QUEUE_PRIORITY_NORMAL或D3D12_COMMAND_QUEUE_PRIORITY_HIGH之一。
	// Flags：命令队列的标志位，可以是D3D12_COMMAND_QUEUE_FLAG_NONE、D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT或D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_IDLE_DEPENDENCY之一。
	// NodeMask：CPU节点掩码，表示该命令队列可以由哪些CPU节点上的线程使用。如果需要在多个CPU节点上并发执行，则需要使用多个命令队列。
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};		// 队列描述
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;				// 指定命令列表的类型(直接命令（直接可以在GPU上执行的命令)
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;				// 指定命令队列的标志（不超时）
	ANALYSIS_RESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

	// 创建分配器
	ANALYSIS_RESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&CommandAllocator)
	));

	// 创建命令列表
	ANALYSIS_RESULT(D3dDevice->CreateCommandList(
		0,		// 默认一个GPU
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,	// 直接GPU执行类型
		CommandAllocator.Get(),		// 将当前命令列表关联到分配器上
		nullptr,	// 需要传入当前管线状态
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())
	));

	// 创建完命令列表，不要忘记将其关闭
	GraphicsCommandList->Close();

}


