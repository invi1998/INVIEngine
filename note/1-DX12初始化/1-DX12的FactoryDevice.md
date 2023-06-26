# DX12的Factory Device

在DirectX 12中，开发者需要使用Factory来创建Device。这是因为在DX12中，Direct3D设备（ID3D12Device）与交换链（IDXGISwapChain3）的创建都需要通过DXGI工厂（IDXGIFactory4）来完成。

具体来说，可以按照以下步骤来创建DX12设备：

1. 首先，创建DXGI工厂。可以使用CreateDXGIFactory1或CreateDXGIFactory2函数来创建DXGI工厂，并将其转换为IDXGIFactory4接口类型。
2. 然后，使用DXGI工厂的EnumAdapters1方法来获取系统上可用的图形适配器列表。这个方法会返回一个指向IDXGIAdapter1接口的指针数组。
3. 接着，遍历图形适配器列表，并使用每个适配器调用D3D12CreateDevice函数来创建Direct3D 12设备。需要注意的是，不同的图形适配器支持的特性级别和硬件资源可能不同，开发者需要根据实际情况选择合适的适配器创建设备。
4. 创建交换链。可以使用IDXGIFactory4::CreateSwapChainForHwnd等函数创建交换链，并指定它的像素格式、分辨率以及刷新率等信息。
5. 最后，创建命令队列、命令分配器、命令列表等对象，并将它们与设备关联起来。这些步骤可以使用ID3D12Device接口提供的相关方法来完成。



## 代码流程

我们需要再程序中创建如下几个对象

```c++
ComPtr<IDXGIFactory4> DXGiFactory;	// 创建DirectX图形基础结构（DXGi）对象
ComPtr<ID3D12Device> D3dDevice;		// 创建命令分配器，命令列表，命令队列，Fence，资源，管道状态对象，堆，根签名，采样器和许多资源视图
ComPtr<ID3D12Fence> Fence;			// 一个用于同步CPU和一个或者多个GPU的对象
```

然后在初始化DirecX函数中，去创建这几个对象

```c++

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


}
```





## Warp高级光栅化平台

DirectX WARP（Windows Advanced Rasterization Platform）是Windows 7及以上操作系统中的一个软件实现的Direct3D 11设备。它可以在不支持硬件加速的情况下运行Direct3D应用程序，以便在低性能或虚拟化环境中使用。

WARP最初是为了让不支持DX11硬件加速的系统也能够运行DX11应用程序而开发的，并且随着DirectX版本的更新，WARP也得到了相应的升级。当前最新版本的WARP是WARP12，它是Direct3D 12 API的一种软件实现，在不支持Direct3D 12硬件功能的系统中提供了基本的Direct3D 12功能。

WARP与硬件设备相比，其主要优势在于可移植性和灵活性。由于WARP是基于软件实现的，因此它可以在任何支持DirectX的Windows操作系统上运行，而不受硬件限制。此外，WARP还可以方便地用于测试、调试和性能分析等使用场景。

不过需要注意的是，WARP由于是基于软件实现的，性能往往会比硬件设备差，特别是在处理大量数据时。因此，对于对性能有较高要求的应用程序，建议还是使用硬件设备来进行加速。



## IID_PPV_ARGS

IID_PPV_ARGS是一种宏定义，用于简化获取接口指针的代码实现。其作用是将指向指针的指针转换为一个IID和一个void类型的指针，并用于参数中。

该宏定义如下：

```c++
#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), reinterpret_cast<void**>(ppType)
```

其中，ppType是一个指向指针的指针，表示要获取的对象的接口类型。IID_PPV_ARGS将ppType转换为两个参数：

- 第一个参数是一个GUID（全局唯一标识符），它通常用于标识对象的接口类型。__uuidof宏可以将类型转换为GUID类型。
- 第二个参数是一个void类型的指针，它指向ppType指向的指针。reinterpret_cast可以将ppType转换为void类型的指针。

使用IID_PPV_ARGS的过程示例：

```c++
ID3D12Device* pDevice;
// ...
HRESULT hr = D3D12CreateDevice(
    nullptr,
    D3D_FEATURE_LEVEL_11_0,
    IID_PPV_ARGS(&pDevice)
);
```

在这个例子中，IID_PPV_ARGS(&pDevice)表示希望获取一个ID3D12Device接口。当D3D12CreateDevice函数执行成功后，pDevice就会指向一个ID3D12Device接口的实例。

总之，使用IID_PPV_ARGS不仅可以简化代码实现，还可以提高代码的可读性和减少错误发生的可能性。